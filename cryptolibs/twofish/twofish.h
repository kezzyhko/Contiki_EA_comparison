#define u32 unsigned int
#define BYTE unsigned char

void encrypt2fish(u32 K[40], u32 S[4][256], BYTE PT[16]);
void decrypt2fish(u32 K[40], u32 S[4][256], BYTE PT[16]);
void keySched(BYTE M[], int N, u32 **S, u32 K[40], int *k);
void fullKey(u32 L[4], int k, u32 QF[4][256]);