#ifndef AES_H
#define AES_H

void setKey(unsigned char* key, int keybits);
void encrypt(unsigned char plaintext[16], unsigned char ciphertext[16]);
void decrypt(unsigned char ciphertext[16], unsigned char plaintext[16]);

#endif