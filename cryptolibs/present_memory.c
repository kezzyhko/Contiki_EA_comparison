#include "crypto.h"
#include "present_memory/present.h"
#include "utils.h"
#include "string.h"

uint32_t* _key;
uint32_t _ive[2] = {0xc0, 0xDE};
uint32_t _ivd[2] = {0xc0, 0xDE};

void setKey(unsigned char* key, int keybits)
{
	_key = (uint32_t*) key;
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor((uint16_t*) _ive, (uint16_t*) plaintext, 4);
	memcpy(ciphertext, _ive, 8);
	present_encrypt( (uint8_t*) ciphertext, (uint8_t*) _key);
	memcpy(_ive, ciphertext, 8);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	memcpy(plaintext, ciphertext, 8);
	present_decrypt( (uint8_t*) plaintext, (uint8_t*) _key);
	xor( (uint16_t*) plaintext, (uint16_t*) _ivd, 4);
	memcpy(_ivd, (uint32_t*) ciphertext, 8);
}