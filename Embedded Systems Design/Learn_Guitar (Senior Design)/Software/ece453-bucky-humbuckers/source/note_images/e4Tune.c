#import "notes.h"

// array size is 9800
const uint16_t e4Tune[]  = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xe73c, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xe71c, 0xe69a, 0xde18, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7,
  0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5b7, 0xd5f7, 0xde7a, 0xe71c, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xe73c, 0xef5d, 0xef5d, 0xef5d, 0xde18, 0xc451, 0xaaaa, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xaa6a, 0xbc10, 0xd5d7, 0xef3c, 0xef5d, 0xef5d, 0xef7d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xef5d, 0xd5f7, 0xaaec, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xaa8a, 0xd596, 0xef5d, 0xef5d, 0xef7d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xe6fb, 0xb36e, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xb2ec, 0xe6ba, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xde9a, 0xaa8a, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa249, 0xde38, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xef7d, 0xef5d, 0xde7a, 0xa229, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1e8, 0xd5f7, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0xef5d, 0xef5d, 0xe6db, 0xaa8a, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa249, 0xe69a, 0xef5d, 0xef7d, 0x0000, 0x0000,
  0x0000, 0x0000, 0xef5d, 0xef5d, 0xb34d, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xaaab, 0xef3c, 0xef5d, 0x0000, 0x0000,
  0x0000, 0xef5d, 0xef5d, 0xd5b7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xccf3, 0xef5d, 0xef7d, 0x0000,
  0x0000, 0xef5d, 0xef5d, 0xaaab, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa249, 0xe6fb, 0xef5d, 0x0000,
  0xe71c, 0xef5d, 0xd5b7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xcd14, 0xef5d, 0xef7d,
  0xef5d, 0xef5d, 0xbbf0, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xb34d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xa249, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0x9986, 0x98a3, 0x98a3, 0x98a3, 0x9945, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1e8, 0xe6fb, 0xef5d,
  0xef5d, 0xe6ba, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0x9883, 0xa9a6, 0xbb6c, 0xa9a6, 0x9021, 0xa1c7, 0xa1c7, 0x99a7, 0x98e4, 0x98a3, 0x98a3, 0x9966, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xd5f7, 0xef5d,
  0xef5d, 0xde38, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0x9945, 0x9000, 0xbb6c, 0xa9a6, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xd575, 0xef5d,
  0xef5d, 0xd596, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0x9925, 0x9000, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xccd3, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945, 0x9945,
  0x9945, 0x9062, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0x9945, 0x9945, 0x9882, 0xb227, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0x98c3, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6, 0xa9a6,
  0xa9a6, 0xa9a6, 0xcc71, 0xef5d, 0xcc71, 0xa9a6, 0xa9a6, 0xa9a6, 0xb248, 0xe658, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbae, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c,
  0xef5c, 0xef5c, 0xef5c, 0xef5d, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5c, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9001, 0xbb2c, 0xef5c, 0xef5d, 0xef1b, 0xd512, 0xcc50, 0xd533, 0xd533, 0xd533, 0xddd5, 0xddd5, 0xddd5, 0xddd5, 0xddd5,
  0xddd5, 0xddd5, 0xe699, 0xef5d, 0xe699, 0xddd5, 0xddd5, 0xddd5, 0xe678, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9905, 0x9021, 0xb2ca, 0xe6da, 0xef5d, 0xef3c, 0xcc70, 0x98a3, 0x9000, 0x9021, 0x9862, 0x9862, 0x9862, 0x9862, 0x9862,
  0x9862, 0x9021, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0x9862, 0x9862, 0x9021, 0xcc90, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9966, 0x9882, 0x98c3, 0xccd2, 0xef5c, 0xef5d, 0xe6da, 0xbaea, 0x9021, 0x98e4, 0x99a7, 0x99c7, 0x99c7, 0x99c7,
  0x99c7, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0x99c7, 0x99c7, 0x98e4, 0x98c3, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9904, 0x9021, 0xb248, 0xe6b9, 0xef5d, 0xef5d, 0xdd94, 0xa124, 0x9062, 0x9966, 0xa1c7, 0xa1c7,
  0xa1c7, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0x9945, 0x9000, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9986, 0x98a3, 0x9882, 0xcc91, 0xef5c, 0xef5d, 0xef1b, 0xc38c, 0x9021, 0x98e4, 0x99a7,
  0xa1c7, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0x9945, 0x9000, 0xbb8d, 0xa9c7, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99c7, 0x9925, 0x9041, 0xaa27, 0xe699, 0xef5d, 0xef5d, 0xddb5, 0xa144, 0x9042,
  0x9945, 0x9882, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0x99a7, 0x98e4, 0x98a3, 0x98a3, 0x9966, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9986, 0x98a3, 0x98a2, 0xcc70, 0xef3c, 0xef5d, 0xef1b, 0xc3cd,
  0x9001, 0x9021, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9945, 0x9041, 0xaa07, 0xe678, 0xef5d, 0xef5d,
  0xde37, 0xa985, 0xbbaf, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x98c3, 0x9041, 0xcc2f, 0xef3c,
  0xef5d, 0xef3c, 0xe658, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99c7, 0x9945, 0x9042, 0xa986,
  0xe637, 0xef5d, 0xef5d, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x98e4,
  0x9041, 0xc40f, 0xef3c, 0xef5d, 0xc3ae, 0x9000, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0x9966, 0x9062, 0xa165, 0xbb6c, 0xa9a6, 0x9021, 0x99a7, 0x99a7, 0x99a7, 0x99c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0x99a7, 0x9904, 0x98a3, 0x9021, 0x9021, 0x9041, 0x9062, 0x9062, 0x9862, 0x9882, 0x9883, 0x9945, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9966, 0x98c3, 0x98c3, 0x98a3, 0x98a3, 0x9882, 0x9882, 0x9882, 0x9945, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9000, 0xbaeb, 0xddb5, 0xd533, 0xd533, 0xbb8d, 0xbb8d, 0xa9a6, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9021, 0xa986, 0xbb0b, 0xbb4c, 0xbb6c, 0xbb6c, 0xbb6d, 0xa9c7, 0x9041, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9000, 0xa1c7, 0xa1c7, 0x99a7, 0x9966, 0x9966, 0x9966,
  0x99c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9021, 0xa9e7, 0xdd94, 0xe6b9, 0xef5c, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef1b, 0xe698, 0xbaeb, 0x9041, 0xa1c7, 0xa1c7, 0x98a3, 0x9000, 0x9000, 0x9001,
  0x9925, 0xa1c7, 0xa1c7, 0xa1c7, 0x9945, 0x98a3, 0x9062, 0x9021, 0xbb4c, 0xef5c, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5c, 0xc38c, 0x9041, 0x9041, 0x9062, 0x9925, 0xa1c7, 0xa1c7, 0x9062, 0xbb8e, 0xef3b, 0xbaea,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99c7, 0x99a7, 0x9966, 0x9021, 0xcc91, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xcc91, 0x9041, 0x9966, 0x99a7, 0xa1a7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98a3, 0xbb6c, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb4c, 0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98c3, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb2b, 0x98e4, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98c3, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb2b, 0x98e4, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98c3, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb2b, 0x98e4, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98c3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98c3, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb2b, 0x98e4, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9062, 0xbbaf, 0xef5d, 0xbb2b,
  0x98a3, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98a3, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbb2b, 0x9041, 0x9882, 0x9882, 0x9882, 0x9882, 0x9882, 0x9882, 0x9021, 0xbbaf, 0xef5d, 0xbb2b,
  0x9021, 0x9882, 0x9882, 0x9882, 0x9882, 0x9882, 0x9882, 0x9882, 0x9021, 0xbb2b, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xd533, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xd575, 0xef5d, 0xd533,
  0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xbb8d, 0xd533, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xef1b, 0xc3ee, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b,
  0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xbb0b, 0xc38d, 0xe6da, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xcd35, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0xbaca, 0x9062, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3,
  0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x98c3, 0x9042, 0xbaca, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc472, 0xef5d,
  0xef5d, 0xd596, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbbaf, 0xef5d, 0x9021, 0x9966, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9904, 0x9021, 0xef5d, 0xbbaf, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xccd3, 0xef5d,
  0xef5d, 0xde18, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9000, 0xbaeb, 0xddd5, 0x9000, 0x9966, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x9945, 0x9000, 0xddd5, 0xbaeb, 0x9062, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xcd55, 0xef5d,
  0xef5d, 0xe6ba, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x98e4, 0x9042, 0x9042, 0x98a3, 0x99a7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x9882, 0x9042, 0x9062, 0x9904, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xd5f7, 0xef5d,
  0xef5d, 0xef3c, 0xa229, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x99a7, 0x99c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0x99a7, 0x99a7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xe6fb, 0xef5d,
  0xef5d, 0xef5d, 0xbbcf, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xb30c, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xd596, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xccd3, 0xef5d, 0xef5d,
  0x0000, 0xef5d, 0xef3c, 0xaa8a, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa229, 0xe6db, 0xef5d, 0x0000,
  0x0000, 0xef5d, 0xef5d, 0xd575, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xc4b2, 0xef5d, 0xef5d, 0x0000,
  0x0000, 0x0000, 0xef5d, 0xef5d, 0xb2ec, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xaa6a, 0xe71c, 0xef5d, 0x0000, 0x0000,
  0x0000, 0x0000, 0xe73c, 0xef5d, 0xe6ba, 0xaa69, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa208, 0xde79, 0xef5d, 0xef5d, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xde39, 0xa208, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xd576, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xde39, 0xaa69, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa229, 0xd5b6, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xe6ba, 0xb2ec, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xaa8a, 0xde7a, 0xef5d, 0xef5d, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xef5d, 0xd575, 0xaa8a, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa249, 0xccf4, 0xef3c, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xe73c, 0xef5d, 0xef5d, 0xef3c, 0xd596, 0xbbcf, 0xa229, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7,
  0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa1c7, 0xa208, 0xbb8e, 0xd555, 0xe71c, 0xef5d, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef3c, 0xe6ba, 0xde18, 0xd596, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35,
  0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xcd35, 0xd575, 0xde18, 0xde9a, 0xe73c, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d,
  0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xef5d, 0xf79e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};