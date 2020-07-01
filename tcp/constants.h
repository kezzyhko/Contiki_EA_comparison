#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PORT 1357

#define SEND_INTERVAL 10

#define MESSAGE_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define MESSAGE_SIZE (10 * 1024)

// For PRESENT and SPECK:
// do not forget to check ../cryptolibs/present.c or ../cryptolibs/speck.c files
// and change configuration macros there

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