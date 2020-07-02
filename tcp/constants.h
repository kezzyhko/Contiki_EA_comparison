#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PORT 1357
#define SEND_INTERVAL 10
#define PACKET_SIZE 64 //512bit

#define MESSAGE_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define MESSAGE_SIZE (10 * 1024)

// For PRESENT, SPECK and TWOFISH:
// do not forget to check ../cryptolibs/present.c, ../cryptolibs/speck.c or ../cryptolibs/twofish.c
// and change configuration macros and includes there as well

// #define BLOCK_LENGTH 4 //32bit
// #define BLOCK_LENGTH 8 //64bit
#define BLOCK_LENGTH 16 //128bit

// #define KEY "c0c0c0c0" //64bit
// #define KEY "c0c0c0c0c0" //80bit
// #define KEY "c0c0c0c0c0c0" //96bit
#define KEY "c0c0c0c0c0c0c0c0" //128bit
// #define KEY "c0c0c0c0c0c0c0c0c0c0c0c0" //192bit
// #define KEY "c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0" //256bit

#endif /* CONSTANTS_H_ */