#include "crypto.h"
#include "string.h"
#include "clefia/clefia_ref.h"


#define MAX_KEYBITS 256

unsigned char _rk[CLEFIA_RK_MAX];
int _nrounds;

unsigned char _ive[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

void setKey(unsigned char* key, int keybits)
{
	_nrounds = ClefiaKeySet(_rk, key, keybits);
}

void xor(unsigned char a[16], const unsigned char b[16]) {
	int i = 17;
	while (--i > 0) {
		*a^=*b;
		++a;
		++b;
	}
}

void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16])
{
	xor(_ive, plaintext);
	ClefiaEncrypt(ciphertext, _ive, _rk, _nrounds);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16])
{
	ClefiaDecrypt(plaintext, ciphertext, _rk, _nrounds);
	xor(plaintext, _ivd);
	memcpy(_ivd, ciphertext, 16);
}