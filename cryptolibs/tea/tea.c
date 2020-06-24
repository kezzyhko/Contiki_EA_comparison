#include "stdint.h"

void encipher(const uint32_t *const v, uint32_t *const w,
   const uint32_t * const k)
{
   register uint32_t 
            y=v[0],
            z=v[1],
            sum=0,
            delta=0x9E3779B9,
            n=32;

   while(n-->0)
   {
      y += (z << 4 ^ z >> 5) + z ^ sum + k[sum&3];
      sum += delta;
      z += (y << 4 ^ y >> 5) + y ^ sum + k[sum>>11 & 3];
   }

   w[0]=y; w[1]=z;
}

void decipher(const uint32_t *const v, uint32_t *const w,
   const uint32_t * const k)
{
   register uint32_t 
            y=v[0],
            z=v[1],
            sum=0xC6EF3720,
				delta=0x9E3779B9,
            n=32;

   /* sum = delta<<5, in general sum = delta * n */

   while(n-->0)
   {
      z -= (y << 4 ^ y >> 5) + y ^ sum + k[sum>>11 & 3];
      sum -= delta;
      y -= (z << 4 ^ z >> 5) + z ^ sum + k[sum&3];
   }
   
   w[0]=y; w[1]=z;
}