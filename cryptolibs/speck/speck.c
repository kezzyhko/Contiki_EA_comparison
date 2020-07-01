/*
  This file defines three functions of SPECK
  1) speck_keyschedule()
     generate the round keys from the secret key)
  2) speck_encryption
     encrypt one plaintext block
  3) speck_decryption
     speck_decrypt
*/

#include <stdint.h>

#define LCS(x,n) ( (x << n) | (x >> ( (SPECK_BLOCK_LENGTH >> 1) - n)) ) // left-rotate a word x by n-bit positions
#define RCS(x,n) ( (x >> n) | (x << ( (SPECK_BLOCK_LENGTH >> 1) - n)) ) // right-rotate a word x by n-bit positions

#if SPECK_BLOCK_LENGTH == 32
    #define R(x,y,k) (x=RCS(x,7), x+=y, x^=k, y=LCS(y,2), y^=x)
    #define InverseR(x,y,k)  ( y ^=x, y=RCS(y,2), x^=k, x-=y, x=LCS(x,7) )
#else
    #define R(x,y,k) (x=RCS(x,8), x+=y, x^=k, y=LCS(y,3), y^=x)
    #define InverseR(x,y,k)  ( y ^=x, y=RCS(y,3), x^=k, x-=y, x=LCS(x,8) )
#endif

#ifndef INT_DEFINED
#define INT_DEFINED
#if SPECK_BLOCK_LENGTH == 32
    #define INT uint16_t
#elif SPECK_BLOCK_LENGTH == 64
    #define INT uint32_t
#else //SPECK_BLOCK_LENGTH == 128
    #define INT uint64_t
#endif
#endif

#define KEYWORDS_SIZE (SPECK_KEY_LENGTH / (SPECK_BLOCK_LENGTH / 2))

//The key schedule
//input:  key,           an array with 2 64-bit words
//output: 32 round keys, an array with 32 64-bit words
void speck_keyschedule(INT key[], INT subkey[])
{
    INT i, B, A;
    
    INT keys[4];
    for (i = 0; i < KEYWORDS_SIZE; ++i)
    {
        keys[i] = key[i];
    }

    subkey[i] = keys[0];

    for(i=0; i < SPECK_ROUNDS - 1; )
    {
        A = keys[0];
        B = keys[1];

        R(B, A, i);

        if (KEYWORDS_SIZE != 2)
        {
            int j;
            for (j = 1; j < (KEYWORDS_SIZE - 1); ++j)
            {
                keys[j] = keys[j+1];
            }
        }
        key[KEYWORDS_SIZE - 1] = B;

        subkey[++i] = keys[0];
    }
}

// encrypt one block of message
// inputs: round keys is an array with 32 64-bit words
//         a plaintext block is an array with 2 64-bit words
// output: a ciphertext block (an array with 2 64-bit words)
void speck_encryption(INT plaintext[], INT ciphertext[], INT subkey[], INT IntVec[])
{
    int i;

    ciphertext[0]=plaintext[0]^IntVec[0];
    ciphertext[1]=plaintext[1]^IntVec[1];

    for(i=0; i < SPECK_ROUNDS; i++)
    {
        R(ciphertext[1], ciphertext[0], subkey[i]);
    }

    //Store ciphertext as IV keys for next round
    IntVec[0] = ciphertext[0];
    IntVec[1] = ciphertext[1];
}

// decrypt one block of message
// inputs: round keys is an array with 32 64-bit words
//         a ciphertext block is an array with 2 64-bit words
// output: a plaintext block (an array with 2 64-bit words)
void speck_decryption(INT plaintext[], INT ciphertext[], INT subkey[], INT IntVec[])
{
    int i;

    plaintext[0]=ciphertext[0];
    plaintext[1]=ciphertext[1];

    for(i=0; i < SPECK_ROUNDS; i++)
    {
        InverseR(plaintext[1], plaintext[0], subkey[SPECK_ROUNDS - 1 - i]);
    }
    plaintext[0] = IntVec[0]^plaintext[0];
    plaintext[1] = IntVec[1]^plaintext[1];

    //Store ciphertext as IV keys for next round
    IntVec[0] = ciphertext[0];
    IntVec[1] = ciphertext[1];
}