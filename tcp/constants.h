#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PORT 1357

#define SEND_INTERVAL 10

#define MESSAGE_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define MESSAGE_SIZE (10 * 1024)

#define KEY "c0c0c0c0c0c0c0c0" //128bit

// #define BLOCK_LENGTH 16 //128bit
#define BLOCK_LENGTH 8 //64bit

#endif /* CONSTANTS_H_ */