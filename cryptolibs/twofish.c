#include "crypto.h"
#include "string.h"
#include "twofish/twofish.h"
#include "stdint.h"

// #define u32 unsigned int
// #define BYTE unsigned char

// u32 *S;
// u32 K[40];
// int k;
// u32 QF[4][256];

#include "twofish/key128.h"

unsigned char _ive[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };


void setKey(unsigned char* key, int keybits)
{
    // keySched(key, 128, &S, K, &k);
    // fullKey(S, k, QF);
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor((uint16_t*) _ive, (uint16_t*) plaintext);
	memcpy(ciphertext, _ive, 16);
	encrypt2fish(K, QF, ciphertext, 8);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	memcpy(plaintext, ciphertext, 16);
	decrypt2fish(K, QF, plaintext);
	xor((uint16_t*) plaintext, (uint16_t*) _ivd, 8);
	memcpy(_ivd, ciphertext, 16);
}