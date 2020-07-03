#include "crypto.h"
#include "DES/des.h"
#include "stdint.h"
#include "string.h"

key_set _k[17];
unsigned char _ive[8] = {0xC0, 0xDE, 0xC1, 0xDD, 0xC2, 0xDC, 0xC3, 0xDB};
unsigned char _ivd[8] = {0xC0, 0xDE, 0xC1, 0xDD, 0xC2, 0xDC, 0xC3, 0xDB};

void setKey(unsigned char* key, int keybits)
{
	generate_sub_keys(key, _k);
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
	xor((uint32_t*) _ive, (uint32_t*) plaintext);
	process_message(_ive, ciphertext, _k, ENCRYPTION_MODE);
	memcpy(_ive, ciphertext, 8);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	process_message(ciphertext, plaintext, _k, DECRYPTION_MODE);
	xor( (uint32_t*) plaintext, (uint32_t*) _ivd);
	memcpy(_ivd, ciphertext, 8);
}