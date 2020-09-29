#ifndef SBWND_INIT_MODULE
#define SBWND_INIT_MODULE


#include <stdint.h>
#include <windows.h>
#include <gdiplus.h>
#include "../sb2/sb_ds.h"
#include "sbwnd_general.h"

// Global variables struct. Currently has: an array of window names, the current first unused id number for windows, the current saucebot global font, and a map that holds all windows currently in use by sbgui.
typedef struct {

	// Array of strings that have window class names to use. Accompanied by #defines for easier use.
	wchar_t **WindowClassNameArray;
	// The current smallest id number not used by sbgui. This is used to make window ids.
	// On a call to initGUI(), this is initialized to 0.
	// On window creation, this number increments.
	uint16_t large_id;
	// The window that functions that get data that require a device context use. This is an invisible top-level window that recieves no messages.
	sbWnd 
		*testWnd,
	// The master window--there can only be one, it can only be created once. If it's closed, the program terminates.
		*masterWnd,
	// The window which currently has the cursor hovering over it.
		*currentCursorWnd;
	// List of all top-level windows that are created by sbgui. All are used in the message loop.
	sbList *TopLevelWindows;
	// Font Family currently used by sbgui, default is Consolas.
	LOGFONTW *currentFont;
	// Global accelerator table for sbgui. Initialized on call to initGUI().
	HACCEL sbAccelTable;
	// Token for Gdi+ startup. Is passed to the shutdown command on call to shutdownGUI( ).
	ULONG_PTR gdipToken;
	// Tells whether windows are created hidden or shown. 1 for showing(default), 0 for hiding.
	uint8_t createMode;
	// Screen x dimension and y dimension
	int
		scrWidth,
		scrHeight;
} SbGUIMasterStruct;

extern SbGUIMasterStruct SbGUIMaster;

#define SBWND_CREATEMODE_SHOW	1
#define SBWND_CREATEMODE_HIDE	0

void initGUI( void );

void initDS( void );

void initWindowFunctions( void );

void initSB( void );

void SetUpSbGUIMaster( void );
void RegisterClasses( void );
void CreateTestWindow( void );

void shutdownSB( void );

#endif