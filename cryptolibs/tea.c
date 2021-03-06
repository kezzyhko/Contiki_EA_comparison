#include "crypto.h"
#include "tea/tea.c"
#include "utils.h"
#include "string.h"

uint32_t* _key;
uint32_t _ive[2] = {0xc0, 0xDE};
uint32_t _ivd[2] = {0xc0, 0xDE};

void setKey(unsigned char* key, int keybits){
	_key = (uint32_t*) key;
}

void encrypt(const unsigned char plaintext[8], unsigned char ciphertext[8])
{
	xor((uint16_t*) _ive, (uint16_t*) plaintext, 4);
	encipher( _ive, (uint32_t*) ciphertext, _key);
	memcpy(_ive, ciphertext, 8);
}


void decrypt(const unsigned char ciphertext[8], unsigned char plaintext[8])
{
	decipher( (uint32_t*) ciphertext, (uint32_t*) plaintext, _key);
	xor((uint16_t*)  plaintext, (uint16_t*) _ivd, 4);
	memcpy(_ivd, (uint32_t*) ciphertext, 8);
}