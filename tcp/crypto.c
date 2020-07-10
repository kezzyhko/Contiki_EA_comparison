#include "constants.h"
#include "algorithms.h"



#if ALGORITHM == AES || ALGORITHM == CLEFIA || ALGORITHM == TWOFISH
	#if BLOCK_LENGTH != 128
		#error Chosen algorithm supports only the following BLOCK_LENGTHs: 128
	#endif
	#if KEY_LENGTH != 128 && KEY_LENGTH != 192 && KEY_LENGTH != 256
		#error Chosen algorithm supports only the following KEY_LENGTHs: 128, 192, 256
	#endif
#endif

#if ALGORITHM == PRESENT_MEMORY || ALGORITHM == PRESENT_SPEED || ALGORITHM == TEA || ALGORITHM == XTEA || ALGORITHM == DES || ALGORITHM == IDEA
	#if BLOCK_LENGTH != 64
		#error Chosen algorithm supports only the following BLOCK_LENGTHs: 64
	#endif
#endif

#if ALGORITHM == TEA || ALGORITHM == XTEA || ALGORITHM == IDEA
	#if KEY_LENGTH != 128
		#error Chosen algorithm supports only the following KEY_LENGTHs: 128
	#endif
#endif

#if ALGORITHM == PRESENT_MEMORY || ALGORITHM == PRESENT_SPEED
	#define CONF_PRESENT (1u)
	#define PRESENT_ROUND_COUNT (31u)
	#if KEY_LENGTH == 80
		#define PRESENT_USE_KEY80  (1u)
		#define PRESENT_USE_KEY128 (0u)
	#elif KEY_LENGTH == 128
		#define PRESENT_USE_KEY80  (0u)
		#define PRESENT_USE_KEY128 (1u)
	#else
		#error Chosen algorithm supports only the following KEY_LENGTHs: 80, 128
	#endif
#endif

// there are more algorithm-specific checks below



#if ALGORITHM == AES
	#include "../cryptolibs/aes.c"
	#include "../cryptolibs/AES/rijndael.c"
#elif ALGORITHM == CLEFIA
	#include "../cryptolibs/clefia.c"
	#include "../cryptolibs/clefia/clefia_ref.c"
#elif ALGORITHM == PRESENT_MEMORY
	#include "../cryptolibs/present_memory.c"
	#include "../cryptolibs/present_memory/PRESENT.c"
#elif ALGORITHM == PRESENT_SPEED
	#include "../cryptolibs/present_speed.c"
	#include "../cryptolibs/present_speed/PRESENT.c"
#elif ALGORITHM == TEA
	#include "../cryptolibs/tea.c"
#elif ALGORITHM == XTEA
	#include "../cryptolibs/xtea.c"
#elif ALGORITHM == SPECK
	#define SPECK_BLOCK_LENGTH BLOCK_LENGTH
	#define SPECK_KEY_LENGTH KEY_LENGTH

	#if BLOCK_LENGTH == 32 && KEY_LENGTH == 64
		#define SPECK_ROUNDS 22
	#elif BLOCK_LENGTH == 64 && KEY_LENGTH == 96
		#define SPECK_ROUNDS 26
	#elif BLOCK_LENGTH == 64 && KEY_LENGTH == 128
		#define SPECK_ROUNDS 27
	#elif BLOCK_LENGTH == 128 && KEY_LENGTH == 128
		#define SPECK_ROUNDS 32
	#elif BLOCK_LENGTH == 128 && KEY_LENGTH == 192
		#define SPECK_ROUNDS 33
	#elif BLOCK_LENGTH == 128 && KEY_LENGTH == 256
		#define SPECK_ROUNDS 34
	#else
		#error Chosen algorithm supports only the following combinations of BLOCK_LENGHT and KEY_LENGTH: 32 and 64, 64 and 96, 64 and 128, 128 and 128, 128 and 192, 128 and 256
	#endif

	#include "../cryptolibs/speck.c"
#elif ALGORITHM == TWOFISH
	#include "stdint.h"
	#define u32 uint32_t
	#define BYTE unsigned char

	#if KEY_LENGTH == 128
		#include "../cryptolibs/twofish/key128.h"
	#elif KEY_LENGTH == 192
		#include "../cryptolibs/twofish/key192.h"
	#elif KEY_LENGTH == 256
		#include "../cryptolibs/twofish/key256.h"
	#endif

	#include "../cryptolibs/twofish.c"
	#include "../cryptolibs/twofish/opt2.c"
#elif ALGORITHM == DES
	#include "../cryptolibs/DES/des.c"
	#if KEY_LENGTH == 56
		#include "../cryptolibs/des.c"
	#elif KEY_LENGTH == 168
		#include "../cryptolibs/3des.c"
	#else
		#error Chosen algorithm supports only the following KEY_LENGTHs: 56, 168
	#endif
#elif ALGORITHM == IDEA
	#include "../cryptolibs/idea.c"
	#include "../cryptolibs/idea/idea.c"
#elif ALGORITHM == SEA
	#if BLOCK_LENGTH != 96
		#error Chosen algorithm supports only the following BLOCK_LENGTHs: 96
	#endif
	#if KEY_LENGTH != 96
		#error Chosen algorithm supports only the following KEY_LENGTHs: 96
	#endif
	#include "../cryptolibs/sea.c"
#else
	#error Please, set ALGORITHM macro to one of the macros defined in algorithms.h
#endif