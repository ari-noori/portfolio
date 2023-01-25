#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;
    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
const int ALIGNMENT = 8;

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 */
void* myAlloc(int size) {
    // Initialize size the "smallest too large" block
    // to be the largest block we feasibly could have
    int bestFitSize = allocsize;

    // Check size - Return NULL if not positive or if larger than heap space.
    if (size < 1 || size > allocsize){
        return NULL;
    }

    // BEST-FIT starts at the front of the heap always
    blockHeader *curr = heapStart;
    // Add the header to the word aligned size (NOTE: MIGHT NOT YET BE WORD ALIGNED)             
    int size_WA = size + sizeof(blockHeader);   

    // Check if the size + header is a multiple of 8 (i.e already word aligned)
    if( (size_WA % ALIGNMENT) != 0){
        // Word align the size. Subtract the remainder from 8 
        // and add it to the size
        size_WA = size_WA + (ALIGNMENT - size_WA % ALIGNMENT);
                                                               
    }

    // A header that might represent the current best fit for the size if we find one.
    blockHeader *bestFit = NULL; 

    // The following loop will iterate through the heap and find an exact fit,
    // or select the best fitting block "bestFit".                                                            
    // While we are NOT at the END:
    while(curr->size_status != 1){
        // Store the size status of the current block
        int size_status = curr->size_status;
        // Get the next header (mask with 3 to remove allocation indicators)
        blockHeader *next = (void*)curr + (size_status & ~3);
        // Size of the alloc indicators is a mask of the lower 2 bits (3)
        int alloc_size = size_status & 3;
        // Get the size of just the block
        int block_size = size_status - alloc_size;
        // Check if the current block is free
        // curr->size_states % 8 ==
        // 0: current free and previous free
        // 1: current occ. and previous free
        // 2: current free and previous occ.
        // 3: current occ. and previous occ.
        if ( (alloc_size == 0) || (alloc_size == 2) ){ 
            // Check if it is an exact match
            // Take the size status and subtract the allocation indicator.
            // If it equals our word aligned size, then it is an exact match.
            if(block_size == size_WA){
                // Change the allocation indicator to represent allocation for NEXT block
                // CAUTION: Do not set the p-bit of the next heap block if the 
                // next heap block is the end mark.
                if(next->size_status != 1){
                    next->size_status += 2; // Add 2 to set previous block allocation 
                }

                // Change the allocation indicator to represent allocation for THIS block
                curr->size_status += 1;
                return curr + 1; // Return the pointer to the payload addr
            }

            // Check if it is too large
            if(block_size > size_WA){
                // Too large!
                // Check to see if it is smaller than the previous too large block
                if (block_size <= bestFitSize){
                    bestFit = curr;
                    bestFitSize = block_size;
                }
                // Set the current header to the next for the next loop iteration.
                curr = next;
            } else {
                // To small! Go onto the next block.
                curr = next;
            }

        } else {
            // Block is NOT free, so go on to the next
            curr = next;
        }
    }

    // After the loop if we haven't returned yet, we must check to see if we
    // can split the bestFit block. If it didn't find a best fit, return null.
    // Check to see if our best fit is larger than our needed alloc-size
    if(NULL != bestFit && size_WA < (bestFit->size_status & ~3)){
        // Modify the size of the block in the header while preserving the allocation
        // indicators. Add one to signify that the block is now allocated.
        bestFit->size_status = (bestFit->size_status & 3) + size_WA + 1;

        // Make a new header for the new block
        // Location of new header is at bestFit + size of alloc'd block
        blockHeader *splitHeader = (void*)bestFit + (bestFit->size_status & ~3);

        // Set allocation indicator for previous block and curr (+2)
        // Size of the block is size of the original best fit - alloc'd block
        splitHeader->size_status = 2 + bestFitSize - (bestFit->size_status & ~3);

        // Create a new footer for the new block
        // Location of the new footer is at splitHeader + size of split block
        // - header
        blockHeader *splitFooter = ((void*)splitHeader + (splitHeader->size_status & ~3)
                                   - sizeof(blockHeader));
        
        // Size of the block footer is just size of header without alloc bits
        splitFooter->size_status = splitHeader->size_status & ~3;
        
        // Return the pointer to the payload
        return bestFit + 1;
    } else {
        return NULL;
    }
    return NULL; // To get rid of warning
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int myFree(void *ptr) {    
    // Check if ptr is NULL, not a mult of 8, or outside heap space
    // Cast pointer to unsigned int to do arithmetic on it
    if(NULL == ptr
      || ((unsigned int)ptr % 8) != 0
      || (unsigned int)ptr > allocsize + (unsigned int)heapStart 
      || ptr < (void*)heapStart){
        return -1;
    }

    // Go back (-4) to the header
    blockHeader *block = ptr - 4;

    // Get the allocation indicators
    int alloc_size = block->size_status & 3;

    // Check if it is already free
    if ( (alloc_size == 0) || (alloc_size == 2) ){
        return -1;
    }

    // Change allocation indicators of current block
    // Clear the MSB to indicate THIS block is free
    block->size_status &= ~1;
    
    // Change the footer to the size of the free block
    // Clear the lower 2 bits to get just the size.
    blockHeader *blockFooter = ((void*)block + (block->size_status & ~3) 
                                - sizeof(blockHeader));
    blockFooter->size_status = block->size_status & ~3;

    // Get the next block
    blockHeader *next = (void*)block + (block->size_status & ~3);

    // Clear the MSB of the allocation indicators
    // CAUTION: Do not set the p-bit of the next heap block if the 
    // next heap block is the end mark. 
    if(next->size_status != 1){
        next->size_status &= ~2;
    } 

    return 0;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    // Number of blocks coalesced
    int count = 0;

    // Start at the front of the heap
    blockHeader *curr = heapStart;

    // Traverse the entire heap until we hit the END mark
    while(curr->size_status != 1){
        // Get the next header (mask with 3 to remove allocation indicators)
        blockHeader *next = (void*)curr + ( curr->size_status & ~3);

        // If the next block has a allocation status of 00, that means the current
        // and next blocks are free. In that case, we can combine them!
        if( (next->size_status & 3) == 0){
            // Keep looping until you find a block that is alloc'd
            while( (next->size_status & 3) == 0){
                // Update current size status
                curr->size_status += next->size_status;
                
                // Set the next block in the chain
                next = (void*)next + ( next->size_status & ~3);
                count++; // increment number of coalesces
            }

            // Set footer
            blockHeader *blockFooter = ((void*)curr + (curr->size_status & ~3) 
                                       - sizeof(blockHeader));
            blockFooter->size_status = curr->size_status & ~3;

            // Set current to the next block and loop again
            curr = (void*)curr + ( curr->size_status & ~3);

        } else {
            // We dont have two adjacent free blocks, so go on to the next block
            curr = next;
        }
    }
	return count;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int myInit(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 

