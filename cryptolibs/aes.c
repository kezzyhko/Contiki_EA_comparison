#include "aes.h"
#include "string.h"
#include "AES/rijndael.h"


#define MAX_KEYBITS 256

unsigned long _rke[RKLENGTH(MAX_KEYBITS)];
unsigned long _rkd[RKLENGTH(MAX_KEYBITS)];
unsigned char _key[KEYLENGTH(MAX_KEYBITS)];
int _keybits;
int _nerounds;
int _ndrounds;

int setKey(char* key, int keybits)
{
	_nerounds = rijndaelSetupEncrypt(_rke, key, keybits);
	_ndrounds = rijndaelSetupDecrypt(_rkd, key, keybits);

	_keybits = keybits;
	memcpy(_key, key, KEYLENGTH(keybits));
}

void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16])
{
	rijndaelEncrypt(_rke, _nerounds, plaintext, ciphertext);
}

void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16])
{
	rijndaelDecrypt(_rkd, _ndrounds, ciphertext, plaintext);
}