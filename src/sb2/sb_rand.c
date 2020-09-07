#include <time.h>
#include "sb2/sb_rand.h"

SbRandomFnStruct 	SbRandom;
SbRandomVarStruct	SbRandomVar;

void initRandom( void ) {
    SbRandoms.seed = seed_SbRandom;
    SbRandoms.rand = rand_SbRandom;
    SbRandomVars.c = 48271;
    SbRandomVars.p = 0x7FFFFFFFL;
    SbRandoms.seed( time( NULL ) );
}

void seed_SbRandom( uint32_t seed ) {
    SbRandomVars.n = seed;
    SbRandoms.rand( );
    SbRandoms.rand( );
}

double rand_SbRandom( void ) {
    SbRandomVars.n = ( SbRandomVars.c * SbRandomVars.n ) % SbRandomVars.p;
    return ( double )SbRandomVars.n / ( double )SbRandomVars.p;
}