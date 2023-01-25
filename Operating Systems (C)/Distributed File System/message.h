#ifndef __message_h__
#define __message_h__

#include "mfs.h"

#define MFS_INIT (1)
#define MFS_LOOKUP (2)
#define MFS_STAT (3)
#define MFS_WRITE (4)
#define MFS_READ (5)
#define MFS_CREAT (6)
#define MFS_UNLINK (7)
#define MFS_SHUTDOWN (8)

#define MAX_NAME_LEN (28)

// This message structure was from Kai's example code
typedef struct __client_message {
    int msg_type;
    union{
        struct {
            char hostname[MAX_NAME_LEN]; // make buffer bigger?
            int port;
        } init;

        struct {
            int pinum;
            char name[MAX_NAME_LEN];
        } lookup;

        struct {
            int inum;
        } stat;

        struct {
            int inum;
            char buffer[MFS_BLOCK_SIZE];
            int offset;
            int num_bytes;
        } write;

        struct {
            int inum;
            char buffer[MFS_BLOCK_SIZE]; // Likely not needed
            int offset;
            int num_bytes;
        } read;

        struct {
            int pinum;
            char name[MAX_NAME_LEN];
            int type;
        } create;

        struct {
            int pinum;
            char name[MAX_NAME_LEN];
        } unlink;
    };
} client_message_t;

// This message structure was from Kai's example code
typedef struct __server_message {
    int return_code;
    union{
        MFS_Stat_t stat;

        struct {
            int pinum;
            char name[MAX_NAME_LEN];
        } lookup;

        struct{
            char buffer[MFS_BLOCK_SIZE];
        } read;
    };
} server_message_t;

// typedef struct {
//     int mtype; // message type from above
//     int rc;    // return code
//     // put more here ...
// } message_t;

#endif // __message_h__
