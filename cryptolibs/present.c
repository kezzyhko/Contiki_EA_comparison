#define CONF_PRESENT (1u) // Do not change this line
#define PRESENT_ROUND_COUNT (31u) // Do not change this line
#define PRESENT_USE_KEY80  (1u) // Disable it if you want to use 128 key length
#define PRESENT_USE_KEY128 (0u) // Enable it if you want to use 128 key length

#include "crypto.h"
#include "present/present.h"
#include "present/PRESENT.c"
#include "stdint.h"
#include "string.h"

uint32_t _ive[2] = {0xc0, 0xDE};
uint32_t _ivd[2] = {0xc0, 0xDE};

uint8_t e_key[32][32];
uint8_t d_key[32][32];

void setKey(unsigned char* key, int keybits)
{
	present_generate_keys(key, e_key, d_key);
}

void xor(uint32_t* a, const uint32_t* b) 
{
	int i = 3;
	while (--i > 0) {
		*a ^= *b;
		++a;
		++b;
	}
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor(_ive, (uint32_t*) plaintext);
	memcpy(ciphertext, _ive, 8);
	present_encrypt( (uint8_t*) ciphertext, e_key);
	memcpy(_ive, ciphertext, 8);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	memcpy(plaintext, ciphertext, 8);
	present_decrypt( (uint8_t*) plaintext, d_key);
	xor( (uint32_t*) plaintext, (uint32_t*) _ivd);
	memcpy(_ivd, (uint32_t*) ciphertext, 8);
}