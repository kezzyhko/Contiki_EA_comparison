#include <stdio.h>
#include "sea.c"

int main() 
{
	uint16_t _rk[NBROUND][2*NB];

	uint8_t key[13] = "c0c0c0c0c0c0";

	KeySchedul(key, _rk);

	printf("const uint16_t _rk[NBROUND][2*NB] = {");

	for (int i = 0; i < NBROUND; ++i)
	{
		printf("\n\t{");
		for (int j = 0; j < 2*NB; ++j)
		{
			if (j % (2*NB) == 0) printf("\n\t\t");
			printf("0x%02x", _rk[i][j]);
			if (j != 2*NB) printf(", ");
		}
		printf("\n\t}");
		if (i != NBROUND) printf(",");
	}
	printf("\n};\n\n");
}