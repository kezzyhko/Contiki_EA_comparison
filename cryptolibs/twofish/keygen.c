#include <stdio.h>
#include "twofish.h"

int main() 
{
	u32 *S;
	u32 K[40];
	int k;
	u32 QF[4][256];

	char key[33] = "c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0";

	keySched(key, 256, &S, K, &k);
	fullKey(S, k, QF);

	int test[2][2] = {{0, 1}, {2, 3}};

	printf("const u32 K[40] = {");
	for (int i = 0; i < 40; ++i)
	{
		if (i % 8 == 0) printf("\n\t");
		printf("0x%08x", K[i]);
		if (i != 39) printf(", ");
	}
	printf("\n};\n\n");

	//QF

	printf("const u32 QF[4][256] = {");

	for (int i = 0; i < 4; ++i)
	{
		printf("\n\t{");
		for (int j = 0; j < 256; ++j)
		{
			if (j % 8 == 0) printf("\n\t\t");
			printf("0x%08x", QF[i][j]);
			if (j != 255) printf(", ");
		}
		printf("\n\t}");
		if (i != 3) printf(",");
	}
	printf("\n};");
}