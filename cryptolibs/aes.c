#include "aes.h"
#include "string.h"
#include "AES/rijndael.h"


#define MAX_KEYBITS 256

unsigned long _rke[RKLENGTH(MAX_KEYBITS)];
unsigned long _rkd[RKLENGTH(MAX_KEYBITS)];
unsigned char _key[KEYLENGTH(MAX_KEYBITS)];
int _keybits;
int _nrounds;

void setKey(unsigned char* key, int keybits)
{
	_nrounds = rijndaelSetupEncrypt(_rke, key, keybits);
	_nrounds = rijndaelSetupDecrypt(_rkd, key, keybits);

	_keybits = keybits;
	memcpy(_key, key, KEYLENGTH(keybits));
}

void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16])
{
	rijndaelEncrypt(_rke, _nrounds, plaintext, ciphertext);
}

void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16])
{
	rijndaelDecrypt(_rkd, _nrounds, ciphertext, plaintext);
}