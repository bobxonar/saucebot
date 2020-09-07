#ifndef SB_INIT_2_MODULE
#define SB_INIT_2_MODULE

#include "sb2/sb_ds.h"
#include <wchar.h>
#include <windows.h>
#include <winhttp.h>

typedef struct {
    wchar_t startDir[ MAX_PATH ];
    sbList *threadHandleList;
    HINTERNET
        hses,
        conSite,
        conCovers,
        conImages;
    
    int cmdOn;
    uint32_t nMax;
} SbMasterStruct;

extern SbMasterStruct SaucebotMaster;

void initSB( void );
void initSB_conTest( void * );

void shutdownSB2( void );

#endif