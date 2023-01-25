#include <stdio.h>
#include "udp.h"
#include "ufs.h"
#include "message.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <math.h>

#define BUFFER_SIZE (1000)

super_t *superblock;    // superblock
inode_t* inodes;        // inode region
int network_fd;         // network file descriptor
int max_inodes;         // max number of inodes
void* fs_img;           // file system image pointer
void* inode_bitmap;     // start of inode bitmap region
void* data_bitmap;      // start of data bitmap region
int file_fd;            // global file descriptor

/**
 * @brief Prints the superblock
 */
void print_superblock(super_t *superblock){
    printf("Superblock:\n");
    printf("inode_bitmap_addr: %x\n", superblock->inode_bitmap_addr);
    printf("inode_bitmap_len: %d\n", superblock->inode_bitmap_len);
    printf("data_bitmap_addr: %x\n", superblock->data_bitmap_addr);
    printf("data_bitmap_len: %d\n", superblock->data_bitmap_len);
    printf("inode_region_addr: %x\n", superblock->inode_region_addr);
    printf("inode_region_len: %d\n", superblock->inode_region_len);
    printf("data_region_addr: %x\n", superblock->data_region_addr);
    printf("data_region_len: %d\n", superblock->data_region_len);
    printf("num_inodes: %d\n", superblock->num_inodes);
    printf("num_data_blocks: %d\n", superblock->num_data);
}

void intHandler(int dummy) {
    printf("Caught SIGINT. Exiting...\n");
    UDP_Close(network_fd);
    exit(130);
}

/**
 * @brief  Returns the inode bitmap for the specified inode number or data block number.
 * 
 * @param bitmap Pointer to the beginning of the inode or data block bitmap region
 * @param position inode or data block number
 * @return unsigned int the bit of the bitmap (0 or 1)
 */ 
unsigned int get_bit(unsigned int *bitmap, int position) {
   int index = position / 32;
   int offset = 31 - (position % 32);
   return (bitmap[index] >> offset) & 0x1;
}

/**
 * @brief Sets the bit of the inode or data block bitmap to 1
 * 
 * @param bitmap Pointer to the beginning of the inode or data block bitmap region
 * @param position inode or data block number
 */
void set_bit(unsigned int *bitmap, int position) {
   int index = position / 32;
   int offset = 31 - (position % 32);
   bitmap[index] |= 0x1 << offset;
}

/**
 * @brief resets the bit of the inode or data block bitmap to 0
 * 
 * @param bitmap Pointer to the beginning of the inode or data block bitmap region
 * @param position inode or data block number
 */
void reset_bit(unsigned int *bitmap, int position) {
   int index = position / 32;
   int offset = 31 - (position % 32);
   bitmap[index] &= ~(0x1 << offset);
}

/**
 * @brief Finds a free inode
 * 
 * @return int free inode number, -1 if none free
 */
int find_free_inode(){
    for(int i = 0; i < max_inodes; i++){
        if(get_bit(inode_bitmap, i) == 0){
            return i;
        }
    }
    return -1;
}

/**
 * @brief Finds a free data block
 * 
 * @return int free data block number, -1 if none free
 */
int find_free_data_block(){
    for(int i = 0; i < superblock->num_data; i++){
        if(get_bit(data_bitmap, i) == 0){
            return i + superblock->data_region_addr;
        }
    }
    return -1;
}

/**
 * @brief Returns some information about the file specified by inum. 
 * Upon success, return 0, otherwise -1. The exact info returned is defined by MFS_Stat_t. 
 * Failure modes: inum does not exist. File and directory sizes are described below.
 * 
 * @param client_message 
 * @param response 
 * @return int return code
 */
int stat_server(client_message_t* client_message, server_message_t* response){
    int inum = client_message->stat.inum;
    if(inum > max_inodes){
        printf("ERROR: Invalid inode number\n");
        response->return_code = -1;
    } else {
        response->stat.type = inodes[inum].type;
        response->stat.size = inodes[inum].size;
        response->return_code = 0;
    }
    return response->return_code;
}

/**
 * @brief takes the parent inode number (which should be the inode number of a directory) 
 * and looks up the entry name in it. The inode number of name is returned. 
 * Success: return inode number of name; failure: return -1. 
 * Failure modes: invalid pinum, name does not exist in pinum.
 * 
 * @param client_message 
 * @param response 
 * @return int return code
 */
int lookup_server(client_message_t* client_message, server_message_t* response){
    int pinum = client_message->lookup.pinum;
    char* name = client_message->lookup.name;
    if(pinum > max_inodes){
        printf("ERROR: Invalid parent inode number\n");
        response->return_code = -1;
        return -1;
    }

    // Get the parent inode
    inode_t* parent_inode = &inodes[pinum];
    //printf("Parent inode type: %d, size: %ld, direct[0]: %d\n", parent_inode->type, parent_inode->size / sizeof(dir_ent_t), parent_inode->direct[0]);

    // Check if it is a directory
    if(parent_inode->type != MFS_DIRECTORY){
        printf("ERROR: Parent inode is not a directory\n");
        response->return_code = -1;
        //printf("rc %d\n", response->return_code);
        return response->return_code;
    }

    // Iterate through all the directory entries in the given parent inode
    // Do this by iterating through all the direct pointers
    // and check the inum of each directory entry in the data block
    for(int i = 0; i < DIRECT_PTRS; i++){
        // Get the data block corresponding to the pointer
        void* data_block = fs_img + parent_inode->direct[i] * UFS_BLOCK_SIZE;

        if(parent_inode->direct[i] == -1){
            // No more data blocks
            continue;
        }

        // Iterate through all the directory entries in the data block
        for(int j = 0; j < (parent_inode->size / sizeof(dir_ent_t)) + 1; j++){
            // Get the directory entry
            dir_ent_t* directory_entry = data_block + j * sizeof(dir_ent_t);

            // Check if the name matches
            if(strcmp(directory_entry->name, name) == 0){
                // Found the directory entry
                response->lookup.pinum = directory_entry->inum;
                //printf("ret inum %d\n", directory_entry->inum);
                response->return_code = 0;
                //printf("rc found! %d\n", response->return_code);
                return response->return_code;
            }
            
        }
    }

    // If we get here, we did not find the directory entry
    printf("ERROR: Directory entry not found\n");
    response->return_code = -1;
    //printf("rc %d\n", response->return_code);
    return -1;
}

/**
 * @brief makes a file (type == MFS_REGULAR_FILE) or directory (type == MFS_DIRECTORY) 
 * in the parent directory specified by pinum of name name. Returns 0 on success, -1 on failure. 
 * Failure modes: pinum does not exist, or name is too long. If name already exists, return success.
 * 
 * @param client_message
 * @param response 
 * @return int return code
 */
int create_server(client_message_t* client_message, server_message_t* response){
    // Extract the essential data from the client message
    int pinum = client_message->create.pinum;
    int type = client_message->create.type;
    char* name = client_message->create.name;

    // Perform sanity checks
    if(pinum > max_inodes){
        printf("ERROR: Invalid parent inode number\n");
        response->return_code = -1;
    }
    if(strlen(name) > 28){
        printf("ERROR: Name is too long\n");
        response->return_code = -1;
        return response->return_code;
    }

    // Get the parent inode and check if it is a directory
    inode_t* parent_inode = &inodes[pinum];
    if(parent_inode->type != MFS_DIRECTORY){
        printf("ERROR: Parent inode is not a directory\n");
        response->return_code = -1;
        return response->return_code;
    }

    // Create a new inode for the file or directory
    int inum = find_free_inode();
    if(inum == -1){
        printf("ERROR: No free inodes\n");
        response->return_code = -1;
        return response->return_code;
    }
    inode_t* new_inode = &inodes[inum];
    new_inode->type = type;
    new_inode->size = 0; // Initialize size to 0 for empty file/directory
    for(int i = 0; i < DIRECT_PTRS; i++){
        new_inode->direct[i] = -1;
    }

    // Only alloc a new block for a directory
    int new_data_block;
    if(type == MFS_DIRECTORY){
        new_data_block = find_free_data_block();
        if(new_data_block == -1){
            printf("ERROR: No free data blocks\n");
            response->return_code = -1;
            return response->return_code;
        }
        // Set this alloc'd block as the first direct pointer
        new_inode->direct[0] = new_data_block;
    }
    

    // Create the new directory entry in the parent directory
    dir_ent_t new_directory_entry;
    strcpy(new_directory_entry.name, name);
    new_directory_entry.inum = inum;

    // Add the new directory entry to the parent directory's data block
    // Iterate through all the direct pointers, and then all the directory entries at that pointer
    for (int i = 0; i < DIRECT_PTRS; i++){
        // Skip over unused direct pointers
        if(parent_inode->direct[i] == -1){
            continue;
        }

        // Get the data block containing the directory entries
        void* data_block = fs_img + parent_inode->direct[i] * UFS_BLOCK_SIZE;

        // update the size of the parent directory
        // This must be done outside the loop so we can loop through all the way to the new one
        parent_inode->size += sizeof(dir_ent_t);

        // Add the new directory entry to the data block of the parent by finding the first unused directory entry
        for(int j = 0; j < parent_inode->size / sizeof(dir_ent_t); j++){
            dir_ent_t* p_directory_entry = data_block + j * sizeof(dir_ent_t);

            // printf("Directory entry name: %s, name to search: %s, inum: %d\n", p_directory_entry->name, name, p_directory_entry->inum);

            // Check if the name already exists
            if(strcmp(p_directory_entry->name, name) == 0){
                printf("ERROR: Directory/file already exists\n");
                parent_inode->size -= sizeof(dir_ent_t); // Dont make a new directory entry
                response->return_code = 0;
                return response->return_code;
            }

            // If the directory entry inum is marked as free, use it!
            if(p_directory_entry->inum == -1){
                // Found an unused directory entry
                p_directory_entry->inum = inum;
                strcpy(p_directory_entry->name, new_directory_entry.name);
                response->return_code = 0;
                // Set the bitmaps
                set_bit(inode_bitmap, inum);

                // Set all inum to -1 for all directory entries in the new directory
                if(type == MFS_DIRECTORY){
                    set_bit(data_bitmap, new_data_block - superblock->data_region_addr);
                    dir_ent_t* new_directory_entry_ptr;
                    void * new_data_block_ptr = fs_img + new_data_block * UFS_BLOCK_SIZE;
                    for(int i = 0; i < UFS_BLOCK_SIZE / sizeof(dir_ent_t); i++){
                        new_directory_entry_ptr = new_data_block_ptr + i * sizeof(dir_ent_t);
                        new_directory_entry_ptr->inum = -1;
                    }

                    // When a directory is created, it should contain two entries: the name . (dot), 
                    // which refers to this new directory's inode number, and .. (dot-dot), 
                    // which refers to the parent directory's inode number.
                    new_directory_entry_ptr = new_data_block_ptr;
                    new_directory_entry_ptr->inum = inum;
                    strcpy(new_directory_entry_ptr->name, ".");
                    new_directory_entry_ptr = new_data_block_ptr + sizeof(dir_ent_t);
                    new_directory_entry_ptr->inum = pinum;
                    strcpy(new_directory_entry_ptr->name, "..");
                    new_inode->size += 2 * sizeof(dir_ent_t);
                }
                return response->return_code;
            }
        }
    }
    printf("ERROR: Could not create!\n");
    response->return_code = -1;
    return -1;
}

/**
 * @brief Reads nbytes of data (max size 4096 bytes) specified by the byte offset 
 * offset into the buffer from file specified by inum. The routine should work 
 * for either a file or directory; directories should return data in the 
 * format specified by MFS_DirEnt_t. Success: 0, failure: -1. 
 * Failure modes: invalid inum, invalid offset, invalid nbytes.
 * 
 * @param client_message 
 * @param response 
 * @return int return code
 */
int read_server(client_message_t* client_message, server_message_t* response){
    int inum = client_message->read.inum;
    int offset = client_message->read.offset;
    int nbytes = client_message->read.num_bytes;

    if(inum > max_inodes){
        printf("ERROR: Invalid inode number\n");
        response->return_code = -1;
        return -1;
    }

    if( (offset + nbytes) > (superblock->num_data-2) * UFS_BLOCK_SIZE){
        printf("ERROR: Exceeds max file size\n");
        fflush(stdout);
        response->return_code = -1;
        return -1;
    }

    inode_t* inode = &inodes[inum];

    int direct_index = offset / UFS_BLOCK_SIZE;

    // Get the data block at the offset
    char* data_block = fs_img + (offset % UFS_BLOCK_SIZE) + inode->direct[direct_index] * UFS_BLOCK_SIZE;

    // Copy all the bytes from the data block into the buffer
    memcpy(response->read.buffer, data_block, nbytes);

    response->return_code = 0;
    return 0;
}

/**
 * @brief writes a buffer of size nbytes (max size: 4096 bytes) at the byte offset 
 * specified by offset. Returns 0 on success, -1 on failure. Failure modes: invalid inum, 
 * invalid nbytes, invalid offset, not a regular file (because you can't write to directories).
 * 
 * @param client_message 
 * @param response 
 * @return int return code
 */
int write_server(client_message_t* client_message, server_message_t* response){
    int inum = client_message->write.inum;
    int offset = client_message->write.offset;
    int nbytes = client_message->write.num_bytes;

    if(inum > max_inodes){
        printf("ERROR: Invalid inode number\n");
        response->return_code = -1;
        return -1;
    }

    // Check if we exceed the max file size (subtract 2 because the root directory needs 1 data block?)
    // I think its minus 2 because there are only 30 ptrs
    // printf("Total Size: %d, Max size: %d\n",(offset + nbytes), (superblock->num_data-2) * UFS_BLOCK_SIZE);
    if( (offset + nbytes) > (superblock->num_data-2) * UFS_BLOCK_SIZE){
        printf("ERROR: Exceeds max file size\n");
        fflush(stdout);
        response->return_code = -1;
        return -1;
    }

    inode_t* inode = &inodes[inum];

    // Check if the inode is a regular file
    if(inode->type != MFS_REGULAR_FILE){
        printf("ERROR: Not a regular file\n");
        response->return_code = -1;
        return -1;
    }

    // Only alloc a new block for a regular file if it has no blocks
    if(inode->type == MFS_REGULAR_FILE && inode->direct[0] == -1){
        int new_data_block = find_free_data_block();
        set_bit(data_bitmap, new_data_block - superblock->data_region_addr);
        if(new_data_block == -1){
            printf("ERROR: No free data blocks\n");
            response->return_code = -1;
            return response->return_code;
        }
        // Set this alloc'd block as the first direct pointer
        inode->direct[0] = new_data_block;
    }

    // If we are writing past the end of the file, allocate more data blocks
    // But only allocate another block if we are writing to more than one block
    if(offset + nbytes > inode->size && offset + nbytes > UFS_BLOCK_SIZE){
        int num_blocks = ceil((offset + nbytes) / UFS_BLOCK_SIZE);
        int num_blocks_allocated = ceil(inode->size / UFS_BLOCK_SIZE);
        int num_blocks_to_allocate = num_blocks - num_blocks_allocated;

        // Allocate the data blocks
        for(int i = 0; i < num_blocks_to_allocate; i++){
            int data_block = find_free_data_block();
            if(data_block == -1){
                printf("ERROR: Could not allocate data block\n");
                response->return_code = -1;
                return -1;
            }
            set_bit(data_bitmap, data_block-superblock->data_region_addr);
            inode->direct[num_blocks_allocated + i] = data_block;
        }
    }

    // printf("Offset: %d\n", offset);

    int direct_index = offset / UFS_BLOCK_SIZE;

    // Get the data block at the offset
    char* data_block = fs_img + offset % UFS_BLOCK_SIZE + inode->direct[direct_index] * UFS_BLOCK_SIZE;

    // Copy all the bytes from the data block into the buffer
    memcpy(data_block, client_message->write.buffer, nbytes);

    // If we are writing past the end of the file, update the size
    inode->size = (offset + nbytes) > inode->size ? (offset + nbytes) : inode->size;

    response->return_code = 0;
    return 0;
}

/**
 * @brief removes the file or directory name from the directory specified by pinum. 0 on success, 
 * -1 on failure. Failure modes: pinum does not exist, directory is NOT empty. 
 * Note that the name not existing is NOT a failure by our definition 
 * (think about why this might be).
 * 
 * @param client_message 
 * @param response 
 * @return int return code
 */
int unlink_server(client_message_t* client_message, server_message_t* response){
    int pinum = client_message->unlink.pinum;
    char* name = client_message->unlink.name;

    if(pinum > max_inodes){
        printf("ERROR: Invalid inode number\n");
        response->return_code = -1;
        return -1;
    }

    inode_t* parent_inode = &inodes[pinum];
    if(parent_inode->type != MFS_DIRECTORY){
        printf("ERROR: Not a directory\n");
        response->return_code = -1;
        return -1;
    }

    // Search for the name in the directory
    for(int i = 0; i < DIRECT_PTRS; i++){
        // Get the data block
        void* data_block = fs_img + parent_inode->direct[i] * UFS_BLOCK_SIZE;

        if(parent_inode->direct[i] == -1){
            continue;
        }

        // Search the data block for the name
        for(int j = 0; j < parent_inode->size; j++){
            // Get the directory entry
            dir_ent_t* directory_entry = data_block + j * sizeof(dir_ent_t);

            if(directory_entry->inum == -1){
                // Directory entry is not used
                continue;
            }

            // Check if the name matches
            if(strcmp(directory_entry->name, name) == 0){
                // Found the name, remove it
                inode_t* inode = &inodes[directory_entry->inum];

                // Check if the directory is empty
                if(inode->type == MFS_DIRECTORY){
                    if(inode->size > 2*sizeof(dir_ent_t)){
                        printf("ERROR: Directory is not empty\n");
                        response->return_code = -1;
                        return -1;
                    }
                }

                // Free the data block if regular file
                if(inode->type == MFS_REGULAR_FILE){
                    for(int k = 0; k < DIRECT_PTRS; k++){
                        if(inode->direct[k] != -1){
                            reset_bit(data_bitmap, inode->direct[k]);
                        }
                    }
                }

                reset_bit(inode_bitmap, directory_entry->inum);
                directory_entry->inum = -1;
                // Decrement the size of the parent directory
                parent_inode->size -= sizeof(dir_ent_t);
                response->return_code = 0;
                return 0;
            }

        }
    }

    // If we get here, the name was not found, but that is not an error
    response->return_code = 0;
    return 0;
}

// server code
int main(int argc, char *argv[]) {
    // A lot of the following code was inspired/copied from Kai's example code
    signal(SIGINT, intHandler);

    assert(argc == 3);
    int port_num = atoi(argv[1]);
    const char* fs_path = argv[2];
    // printf("%s\n", fs_path);

    network_fd = UDP_Open(port_num);
    assert(network_fd > -1);

    // Open the file system image
    file_fd = open(fs_path, O_RDWR | O_APPEND, S_IRWXU);
    if(file_fd == -1){
        printf("ERROR: Couldn't open the file!\n");
        exit(1);
    }

    // Get the file system image size
    struct stat finfo;
    assert(fstat(file_fd, &finfo) == 0);

    // Map the file system image into memory
    // Whenever you make changes, call msync
    fs_img = mmap(NULL, finfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);
    assert(fs_img != MAP_FAILED);

    // Set pointer to superblock
    superblock = (super_t*)fs_img;

    // Set pointer to inode region
    inodes = (inode_t*)(fs_img + superblock->inode_region_addr * UFS_BLOCK_SIZE);

    // Set pointer to bitmap region
    inode_bitmap = (void*)(fs_img + superblock->inode_bitmap_addr * UFS_BLOCK_SIZE);
    data_bitmap = (void*)(fs_img + superblock->data_bitmap_addr * UFS_BLOCK_SIZE);

    // Set the first data block as in use because the root directory uses it
    set_bit(data_bitmap, superblock->data_region_addr);
    msync(fs_img, finfo.st_size, MS_SYNC);

    // Set max inodes
    max_inodes = superblock->num_inodes;
    //printf("max inodes: %d\n", max_inodes);

    while(1){
        struct sockaddr_in client_addr;
        client_message_t* client_message = malloc(sizeof(client_message_t));
        server_message_t* response = malloc(sizeof(server_message_t));

        // Read the incoming message
        UDP_Read(network_fd, &client_addr, (void*)client_message, sizeof(client_message_t));
        
        if(client_message->msg_type == MFS_STAT){
            stat_server(client_message, response);
        } else if(client_message->msg_type == MFS_LOOKUP){
            //printf("Lookup\n");
            lookup_server(client_message, response);
        } else if (client_message->msg_type == MFS_CREAT){
            //printf("Create\n");
            create_server(client_message, response);
            msync(fs_img, finfo.st_size, MS_SYNC);
        } else if(client_message->msg_type == MFS_READ){
            //printf("Read\n");
            read_server(client_message, response);
        } else if(client_message->msg_type == MFS_WRITE){
            //printf("Write\n");
            write_server(client_message, response);
            msync(fs_img, finfo.st_size, MS_SYNC);
        } else if(client_message->msg_type == MFS_SHUTDOWN){
            msync(fs_img, finfo.st_size, MS_SYNC);
            free(client_message);
            free(response);
            //printf("Shutting down...\n");
            response->return_code = 0;
            UDP_Write(network_fd, &client_addr, (void*)response, sizeof(server_message_t));
            UDP_Close(network_fd);
            exit(0);
        } else if(client_message->msg_type == MFS_UNLINK){
            //printf("Unlink\n");
            unlink_server(client_message, response);
            msync(fs_img, finfo.st_size, MS_SYNC);
        } else {
            printf("ERROR: Invalid message type\n");
            response->return_code = -1;
        }

        UDP_Write(network_fd, &client_addr, (void*)response, sizeof(server_message_t));
    }
    return -1; 
}
    


 
