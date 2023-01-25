/*
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/
  
const long long int ALL_ONES = 0xFFFFFFFFFFFFFFFF;  // A large number of all 1s

//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.
//TODO - COMPLETE THIS TYPE
typedef struct cache_line {                    
    char valid;
    mem_addr_t tag;
    //Add a data member as needed by your implementation for LRU tracking.
    int cnt;
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;

//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache we're simulating. 
//Note: A cache is a pointer to a heap array of one or more cache sets.
cache_t cache;  

/* init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {
    // Initialize the global variable based on inputs
    S = 1 << s; // 2^s (Example: 1 << 4 = 10000 = 2^4)  

    // Alloc memory for cache (and exit if it fails)
    if ( NULL == (cache = malloc(S * sizeof(cache_set_t))) ){
        printf("ERROR: Unable to allocate memory for the cache. Please try again.\n");
        exit(1);
    }   

    // Now allocate each line in the cache
    // Iterate through each set and allocate the proper number of lines
    for (int i = 0; i < S; i++){
        if ( NULL == (cache[i] = malloc(E * sizeof(cache_line_t))) ) {
            printf("ERROR: Unable to allocate memory for the cache LINES. Please try again.\n");
            exit(1);
        }

        // Initialize the valid, tag, and LRU cnt fields for each entry
        // Setting to zero improves security and potentially catches edge cases
        for (int k = 0; k < E; k++){
            cache[i][k].cnt = 0;
            cache[i][k].tag = 0;
            cache[i][k].valid = 0;
        }
    }
}
  

/* free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {
    // Work backwards from the order of alloc to free all memory
    for (int i = 0; i < S; i++){
        free(cache[i]);
        cache[i] = NULL; // Make sure pointer doesnt dangle
    }
    free(cache);
    cache = NULL;
    // Checked mem-leaks with valgrind
}

/**
 * getSetBits:
 * @brief Gets the set bits from the entire address.
 * 
 * @param addr The address to extract the set bits from
 * @return mem_addr_t the set bits from the address
 */
mem_addr_t getSetBits(mem_addr_t addr){
    // Right shifts out the b-bits and then masks that number with the number of s bits
    // Example: s = 2 -> 0x...11111111 -> 0x...11111100 -> 0x...00000011
    return (addr >> b) & ~(ALL_ONES << s);
}

/**
 * getTagBits:
 * @brief Gets the tag bits from the entire address.
 * 
 * @param addr The address to extract the set bits from
 * @return mem_addr_t the tag bits from the address
 */
mem_addr_t getTagBits(mem_addr_t addr){
    // Simply perform a right shift to kill all the lower bits
    return addr >> (s+b);
}

/**
 * incrementLRU:
 * @brief Finds and increments the largest LRU counter in the given set.
 * Assumes the data was already validated before incrementing the LRU counter.
 * 
 * @param addr The address of the data
 * @return int the largest LRU value plus 1
 */
int incrementLRU (mem_addr_t addr){
    cache_set_t currSet = cache[getSetBits(addr)]; // save current set
    int largestLRU = 0; // There is no largest LRU yet, begin at 0
    for (int i = 0; i < E; i++){    // Iterate through all lines
        if((currSet[i].cnt > largestLRU) && currSet[i].valid){ // Check for new largest LRU cnt
            largestLRU = currSet[i].cnt;
        }
    }
    return largestLRU + 1; // Return the incremented LRU value for easy assigning to line. 
}

/**
 * checkHit:
 * @brief Checks if there is a cache hit with the given address
 * 
 * @param addr the address of the data to check
 * @return int 1 if there was a hit, 0 if there was not
 */
int checkHit(mem_addr_t addr){
    cache_set_t currSet = cache[getSetBits(addr)];  // Save our current set
    for (int i = 0; i < E; i++){ // Iterate through all lines
        // Check if it line is valid and tags match
        if((getTagBits(addr) == currSet[i].tag) && currSet[i].valid){
            // Set that line as the most recently used
            currSet[i].cnt = incrementLRU(addr);
            return 1;
        }
    }
    return 0;	
}

/**
 * checkVacancy:
 * @brief Checks if there is a vacant line in the given cache set
 * 
 * @param addr the address of the data to check
 * @return int 1 if a vacancy was filled, 0 otherwise
 */
int checkVacancy(mem_addr_t addr){
    cache_set_t currSet = cache[getSetBits(addr)];  // Save our current set
    for (int i = 0; i < E; i++){ // Iterate through all lines
        if(!currSet[i].valid){ // Vacancy when a line is not valid
            currSet[i].valid = 1; // Validate line
            currSet[i].tag = getTagBits(addr); // Assign tag bits
            currSet[i].cnt = incrementLRU(addr); // set as most recently used
            return 1;
        }
    }
    return 0;
}

/**
 * getLRU:
 * @brief Gets the current LRU line (the smallest LRU counter value)
 * 
 * @param addr the address of the data to find the LRU
 * @return int the smallest LRU
 */
int getLRU(mem_addr_t addr){
    int currLRU = INT_MAX; // Assign to largest number so we can do initial comparison
    for (int i = 0; i < E; i++){
        cache_line_t currLine = cache[getSetBits(addr)][i]; // save current line
        if(currLRU > currLine.cnt){ // Check if current LRU is less than previous
            currLRU = currLine.cnt; // and update the current LRU count to be returned
        }
    }
    return currLRU;
}

/**
 * evict:
 * @brief Evicts a cache line and replaces it with new data. This method assumes
 * that you have checked for hits/vacancies first.  
 * 
 * @param addr the address with the data you want to store
 */
void evict(mem_addr_t addr){
    cache_set_t currSet = cache[getSetBits(addr)]; // Save the current set
    for (int i = 0; i < E; i++){
        // If the current line in the set is the LRU, then we will evict it
        // If there is a tie, the first LRU line gets kicked out.
        if(getLRU(addr) == currSet[i].cnt){ 
            currSet[i].valid = 1;   // Validate line
            currSet[i].tag = getTagBits(addr);  // assign tag
            currSet[i].cnt = incrementLRU(addr); // assign most recently used
            return; // Stop iterating after eviction.
        }
    }
}


/* access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr) {
    // First, check for a hit. If we hit, we are done.
    if (checkHit(addr)) {
        hit_cnt ++;
        return;
    }	

    // Since we didnt hit, increment the miss count
	miss_cnt++;

    // Now check if we have a vacancy (invalid line)
    if (checkVacancy(addr)){
        return;
    }

    // Since we didn't have a vacancy, we must evict.
	evict_cnt++;

    // Evict a line
    evict(addr);
}


/* replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);
      
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);
            access_data(addr);
            // Call again if Modify
            if(buf[1] == 'M'){
                access_data(addr);
            }

            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}  
  
  
/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for block offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  
  
  
/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  
  
  
/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    //Initialize cache.
    init_cache();

    //Replay the memory access trace.
    replay_trace(trace_file);

    //Free memory allocated for cache.
    free_cache();

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  