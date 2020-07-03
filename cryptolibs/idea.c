#include "crypto.h"
#include "string.h"
#include "idea/idea.h"
#include "utils.h"

#define MAX_KEYBITS 256

IdeaContext _ic; 

unsigned char _ive[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
unsigned char _ivd[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

void setKey(unsigned char* key, int keybits)
{
	ideaInit(&_ic, key, keybits);
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor( (uint16_t*) _ive, (uint16_t*) plaintext, 8);
	ideaEncryptBlock(&_ic, _ive, ciphertext);
	memcpy(_ive, ciphertext, 16);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	ideaDecryptBlock(&_ic, ciphertext, plaintext);
	xor( (uint16_t*) plaintext, (uint16_t*) _ivd, 8);
	memcpy(_ivd, ciphertext, 16);
}