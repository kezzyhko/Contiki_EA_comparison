#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include "algorithms.h"

#define PORT 1357
#define SEND_INTERVAL 10 //seconds
#define PACKET_SIZE 64 //bytes

#define MESSAGE_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define MESSAGE_SIZE (10 * 1024)

#define ALGORITHM XTEA
#define BLOCK_LENGTH 64
#define KEY_LENGTH 128

#define KEY "c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0"
// it's 256bit, but only first KEY_LENGTH bits will be used
// TWOFISH algorithm uses precalculated keys located in "../cryptolibs/twofish"

#endif /* CONSTANTS_H_ */