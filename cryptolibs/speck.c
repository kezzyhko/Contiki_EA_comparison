#define SPECK_BLOCK_LENGTH 128
#define SPECK_KEY_LENGTH 128
#define SPECK_ROUNDS 32

#include "crypto.h"
#include "string.h"
#include "speck/speck.c"
#include "stdint.h"

#ifndef INT_DEFINED
#define INT_DEFINED
#if SPECK_BLOCK_LENGTH == 32
    #define INT uint16_t
#elif SPECK_BLOCK_LENGTH == 64
    #define INT uint32_t
#else //SPECK_BLOCK_LENGTH == 128
    #define INT uint64_t
#endif
#endif

INT _rk[SPECK_ROUNDS];
INT _ive[2] = {0, 0};
INT _ivd[2] = {0, 0};

void setKey(unsigned char* key, int keybits)
{
	speck_keyschedule((INT*)key, _rk);
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	speck_encryption((INT*)plaintext, (INT*)ciphertext, _rk, _ive);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	speck_decryption((INT*)plaintext, (INT*)ciphertext, _rk, _ivd);
}