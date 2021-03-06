#include "crypto.h"
#include "DES/des.h"
#include "utils.h"
#include "string.h"

key_set _k[17];
unsigned char _ive[8] = {0xC0, 0xDE, 0xC1, 0xDD, 0xC2, 0xDC, 0xC3, 0xDB};
unsigned char _ivd[8] = {0xC0, 0xDE, 0xC1, 0xDD, 0xC2, 0xDC, 0xC3, 0xDB};

void setKey(unsigned char* key, int keybits)
{
	generate_sub_keys(key, _k);
}

void encrypt(const unsigned char* plaintext, unsigned char* ciphertext)
{
	xor((uint16_t*) _ive, (uint16_t*) plaintext, 4);
	process_message(_ive, ciphertext, _k, ENCRYPTION_MODE);
	memcpy(_ive, ciphertext, 8);
}

void decrypt(const unsigned char* ciphertext, unsigned char* plaintext)
{
	process_message(ciphertext, plaintext, _k, DECRYPTION_MODE);
	xor( (uint16_t*) plaintext, (uint16_t*) _ivd, 4);
	memcpy(_ivd, ciphertext, 8);
}