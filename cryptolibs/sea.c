#include "crypto.h"
#include "string.h"
#include "sea/sea.c"
#include "utils.h"

#include "sea/key.h"

uint16_t _ive[NB * 2];
uint16_t _ivd[NB * 2];

void setKey(unsigned char* key, int keybits)
{
	KeySchedul((uint16_t *) key, _rk);
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor( (uint16_t*) _ive, (uint16_t*) plaintext, NB * 2);
	memcpy(ciphertext, _ive, NB * 2 * 2);
	Encrypt((uint16_t *) ciphertext, _rk);
	memcpy(_ive, ciphertext, NB * 2 * 2);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	memcpy(plaintext, ciphertext, NB * 2 * 2);
	Decrypt((uint16_t *) plaintext, _rk);
	xor( (uint16_t*) plaintext, (uint16_t*) _ivd, NB * 2);
	memcpy(_ivd, ciphertext, NB * 2 * 2);
}