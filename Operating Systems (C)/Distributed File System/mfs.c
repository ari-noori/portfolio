// Shared library libmfs.so

#include "mfs.h"
#include "ufs.h"
#include "message.h"
#include "udp.h"
#include <time.h>
#include <stdlib.h>

char* global_hostname;
int global_port;

struct sockaddr_in server_addr;
int fd;

// Netowrk call function


// Takes a host name and port number and uses those to find the server exporting the file system.
int MFS_Init(char *hostname, int port){
    int rc = UDP_FillSockAddr(&server_addr, hostname, port);
    assert(rc == 0);

    int MIN_PORT = 20000;
    int MAX_PORT = 40000;

    srand(time(0));
    int port_num = (rand() % (MAX_PORT - MIN_PORT) + MIN_PORT);
    fd = UDP_Open(port_num);
    
    global_hostname = hostname;
    global_port = port;
    
    return 0;
}

/*
Takes the parent inode number (which should be the inode number of a directory) 
and looks up the entry name in it. The inode number of name is returned. 
Success: return inode number of name; failure: return -1. 
Failure modes: invalid pinum, name does not exist in pinum.
*/
int MFS_Lookup(int pinum, char *name){
    client_message_t message;
    message.msg_type = MFS_LOOKUP;
    message.lookup.pinum = pinum;
    strcpy(message.lookup.name, name);

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    if(response.return_code == -1){
        return -1;
    }
    printf("Client RC: %d\n", response.return_code);
    printf("Ret inum client %d\n", response.lookup.pinum);
    return response.lookup.pinum;
}

/*
Returns some information about the file specified by inum. Upon success, return 0, 
otherwise -1. The exact info returned is defined by MFS_Stat_t. 
Failure modes: inum does not exist. File and directory sizes are described below.
*/
int MFS_Stat(int inum, MFS_Stat_t *m){
    // The code below was taken from Kai's example code
    client_message_t message;
    message.msg_type = MFS_STAT;
    message.stat.inum = inum;

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));


    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    m->type = response.stat.type;
    m->size = response.stat.size;
    return response.return_code;
}

/*
Writes a buffer of size nbytes (max size: 4096 bytes) at the byte offset specified 
by offset. Returns 0 on success, -1 on failure. Failure modes: invalid inum, invalid nbytes, 
invalid offset, not a regular file (because you can't write to directories).
*/
int MFS_Write(int inum, char *buffer, int offset, int nbytes){
    client_message_t message;
    message.msg_type = MFS_WRITE;
    message.write.inum = inum;
    message.write.offset = offset;
    message.write.num_bytes = nbytes;
    memcpy(message.write.buffer, buffer, nbytes);

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    return response.return_code;
}


/*
Reads nbytes of data (max size 4096 bytes) specified by the byte offset offset 
into the buffer from file specified by inum. The routine should work for either 
a file or directory; directories should return data in the format specified by 
MFS_DirEnt_t. Success: 0, failure: -1. 
Failure modes: invalid inum, invalid offset, invalid nbytes.
*/
int MFS_Read(int inum, char *buffer, int offset, int nbytes){
    client_message_t message;
    message.msg_type = MFS_READ;
    message.read.inum = inum;
    message.read.offset = offset;
    message.read.num_bytes = nbytes;

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    if(response.return_code == -1){
        return -1;
    }
    memcpy(buffer, response.read.buffer, nbytes);
    
    return 0;
}

/*
Makes a file (type == MFS_REGULAR_FILE) or directory (type == MFS_DIRECTORY) 
in the parent directory specified by pinum of name name. Returns 0 on success, 
-1 on failure. Failure modes: pinum does not exist, or name is too long. 
If name already exists, return success.
*/
int MFS_Creat(int pinum, int type, char *name){
    client_message_t message;
    message.msg_type = MFS_CREAT;
    message.create.pinum = pinum;
    message.create.type = type;
    strcpy(message.create.name, name);

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    return response.return_code;
}

/*
removes the file or directory name from the directory specified by pinum. 0 on success, 
-1 on failure. Failure modes: pinum does not exist, directory is NOT empty. 
Note that the name not existing is NOT a failure by our definition (think about why this might be).
*/
int MFS_Unlink(int pinum, char *name){
    client_message_t message;
    message.msg_type = MFS_UNLINK;
    message.unlink.pinum = pinum;
    strcpy(message.unlink.name, name);

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    return response.return_code;
}

/*
Tells the server to force all of its data structures to disk and shutdown
by calling exit(0). This interface will mostly be used for testing purposes.
*/  
int MFS_Shutdown(){
    client_message_t message;
    message.msg_type = MFS_SHUTDOWN;

    UDP_Write(fd, &server_addr, (void*)&message, sizeof(client_message_t));

    server_message_t response;
    struct sockaddr_in ret_addr;
    UDP_Read(fd, &ret_addr, (void*)&response, sizeof(server_message_t));
    UDP_Close(fd);
    return response.return_code;
}