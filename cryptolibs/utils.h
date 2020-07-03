#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"

void xor(uint16_t* a, const uint16_t* b, int size) 
{
	int i = size;
	while (i > 0) {
		*a ^= *b;
		++a;
		++b;
		--i;
	}
}

#endif