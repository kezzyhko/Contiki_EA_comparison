#ifndef CRYPTO_H
#define CRYPTO_H

void setKey(unsigned char* key, int keybits);
void encrypt(const unsigned char* plaintext, unsigned char* ciphertext);
void decrypt(const unsigned char* ciphertext, unsigned char* plaintext);

#endif