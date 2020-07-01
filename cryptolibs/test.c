#include <stdio.h>
#include "crypto.h"


int main()
{
	printf("%ld\n", sizeof(long));
	unsigned char plaintext1[17] = "test1234test1234";
	unsigned char ciphertext1[17];

	unsigned char plaintext2[17] = "lolkekchlolkekch";
	unsigned char ciphertext2[171];

	printf("1\n");

	char key[16] = "c0c0c0c0c0c0c0c0";

	printf("1\n");

	setKey(key, 128);

	printf("1\n");

	encrypt(plaintext1, ciphertext1);
	encrypt(plaintext2, ciphertext2);
	
	printf("1\n");

	printf("%p\n", &ciphertext1);
	printf("%s\n", (char*) &plaintext1);

	printf("%p\n", &ciphertext2);
	printf("%s\n", (char*) &plaintext2);

	printf("1\n");

	decrypt(ciphertext1, plaintext2);
	decrypt(ciphertext2, plaintext1);

	printf("1\n");

	printf("%s\n", (char *) &plaintext1);
	printf("%s\n", (char *) &plaintext2);
}