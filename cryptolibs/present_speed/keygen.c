#define CONF_PRESENT (1u)
#define PRESENT_ROUND_COUNT (31u)
#define PRESENT_USE_KEY80  (0u)
#define PRESENT_USE_KEY128 (1u)

#include <stdio.h>
#include "present.c"

int main() 
{
	uint8_t ke[32][32];
	uint8_t kd[32][32];

	uint8_t key[33] = "c0c0c0c0c0c0c0c0";

	present_generate_keys(key, ke, kd);

	printf("const uint8_t ke[32][32] = {");

	for (int i = 0; i < 32; ++i)
	{
		printf("\n\t{");
		for (int j = 0; j < 32; ++j)
		{
			if (j % 8 == 0) printf("\n\t\t");
			printf("0x%02x", ke[i][j]);
			if (j != 31) printf(", ");
		}
		printf("\n\t}");
		if (i != 31) printf(",");
	}
	printf("\n};\n\n");

	printf("const uint8_t kd[32][32] = {");

	for (int i = 0; i < 32; ++i)
	{
		printf("\n\t{");
		for (int j = 0; j < 32; ++j)
		{
			if (j % 8 == 0) printf("\n\t\t");
			printf("0x%02x", kd[i][j]);
			if (j != 31) printf(", ");
		}
		printf("\n\t}");
		if (i != 31) printf(",");
	}
	printf("\n};\n\n");
}