#include "crypto.h"
#include "string.h"
#include "AES/rijndael.h"


#define MAX_KEYBITS 256

unsigned long _rke[RKLENGTH(MAX_KEYBITS)];
unsigned long _rkd[RKLENGTH(MAX_KEYBITS)];
int _nrounds;

unsigned char _ive[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

void setKey(unsigned char* key, int keybits)
{
	_nrounds = rijndaelSetupEncrypt(_rke, key, keybits);
	_nrounds = rijndaelSetupDecrypt(_rkd, key, keybits);
}

void xor(unsigned int* a, const unsigned int* b) {
	int i = 5;
	while (--i > 0) {
		*a ^= *b;
		++a;
		++b;
	}
}

void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16])
{
	xor( (unsigned int*) _ive, (unsigned int*) plaintext);
	rijndaelEncrypt(_rke, _nrounds, _ive, ciphertext);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16])
{
	rijndaelDecrypt(_rkd, _nrounds, ciphertext, plaintext);
	xor( (unsigned int*) plaintext, (unsigned int*) _ivd);
	memcpy(_ivd, ciphertext, 16);
}