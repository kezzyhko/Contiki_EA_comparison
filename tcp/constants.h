#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include "algorithms.h"

#define PORT 1357
#define SEND_INTERVAL 10 //seconds
#define PACKET_SIZE 64 //bytes

#define MESSAGE_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define MESSAGE_SIZE (10 * 1024)

#define ALGORITHM SEA
#define BLOCK_LENGTH 96
#define KEY_LENGTH 96

#define KEY "c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0"
// it's 256bit, but only first KEY_LENGTH bits will be used
// TWOFISH and PRESENT_SPEED algorithms use precalculated keys located in "../cryptolibs/twofish" and "../cryptolibs/present_speed"

#endif /* CONSTANTS_H_ */