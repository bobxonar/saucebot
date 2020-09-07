#ifndef SB_RAND_MODULE
#define SB_RAND_MODULE

#include <stdint.h>

typedef struct {
	void ( *seed )( uint32_t );
	double ( *rand )( void );
} SbRandomFnStruct;

typedef struct {
	uint32_t
		n,
		p,
		c;
} SbRandomVarStruct;

extern SbRandomFnStruct 	SbRandom;
extern SbRandomVarStruct	SbRandomVar;

#define SbRandoms			SbRandom
#define SbRandomVars		SbRandomVar

void initRandom( void );
void seed_SbRandom( uint32_t );
double rand_SbRandom( void );

#endif