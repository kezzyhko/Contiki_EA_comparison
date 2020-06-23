#include "aes.h"
#include "string.h"
#include "AES/rijndael.h"


#define MAX_KEYBITS 256

unsigned long _rke[RKLENGTH(MAX_KEYBITS)];
unsigned long _rkd[RKLENGTH(MAX_KEYBITS)];
unsigned char _key[KEYLENGTH(MAX_KEYBITS)];
int _keybits;
int _nrounds;

unsigned char _ive[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[32] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

void setKey(unsigned char* key, int keybits)
{
	_nrounds = rijndaelSetupEncrypt(_rke, key, keybits);
	_nrounds = rijndaelSetupDecrypt(_rkd, key, keybits);

	_keybits = keybits;
	memcpy(_key, key, KEYLENGTH(keybits));
}

void xor(unsigned char a[16], unsigned char b[16]) {
	int i = 17;
	while (--i > 0) {
		*a^=*b;
		++a;
		++b;
	}
}

void encrypt(unsigned char plaintext[16], unsigned char ciphertext[16])
{
	xor(plaintext, _ive);
	rijndaelEncrypt(_rke, _nrounds, plaintext, ciphertext);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(unsigned char ciphertext[16], unsigned char plaintext[16])
{
	rijndaelDecrypt(_rkd, _nrounds, ciphertext, plaintext);
	xor(plaintext, _ivd);
	memcpy(_ivd, ciphertext, 16);
}