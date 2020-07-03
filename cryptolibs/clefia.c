#include "crypto.h"
#include "string.h"
#include "clefia/clefia_ref.h"
#include "utils.h"

unsigned char _rk[CLEFIA_RK_MAX];
int _nrounds;

unsigned char _ive[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

void setKey(unsigned char* key, int keybits)
{
	_nrounds = ClefiaKeySet(_rk, key, keybits);
}

void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16])
{
	xor( (uint16_t*) _ive, (uint16_t*) plaintext, 8);
	ClefiaEncrypt(ciphertext, _ive, _rk, _nrounds);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16])
{
	ClefiaDecrypt(plaintext, ciphertext, _rk, _nrounds);
	xor( (uint16_t*) plaintext, (uint16_t*) _ivd, 8);
	memcpy(_ivd, ciphertext, 16);
}