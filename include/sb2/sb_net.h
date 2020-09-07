#ifndef SBNET_MODULE
#define SBNET_MODULE

// Header file for saucebot's scraper. Interacts with nhentai to get raw html text for parsing, and can download images.

typedef struct {
    // Gets the information for a number or page of a number.
    void ( *numget )( wchar_t *, wchar_t * );
    // Gets one image. Supply one URL and one path.
    void ( *imgget )( char *, wchar_t * );
    // Gets one image from t.nhentai.net. Supply one url and one path. Uses wide strings.
    void ( *imgget_tsite )( wchar_t *, wchar_t * );
} SbNetFnStruct;

extern SbNetFnStruct SbNet;

void initNet( void );

// Gets an image into the file specified.
void numget_winhttp( wchar_t *, wchar_t * );
void imgget_winhttp( char *, wchar_t * );
void imgget_tsite_winhttp( wchar_t *, wchar_t * );

#endif
