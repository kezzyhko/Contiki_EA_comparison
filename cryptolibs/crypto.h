#ifndef CRYPTO_H
#define CRYPTO_H

void setKey(unsigned char* key, int keybits);
void encrypt(const unsigned char plaintext[16], unsigned char ciphertext[16]);
void decrypt(const unsigned char ciphertext[16], unsigned char plaintext[16]);

#endif