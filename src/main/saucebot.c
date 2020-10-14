#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <string.h>
#include <process.h>

#include "sbwnd/sbgui.h"
#include "sb2/sb2.h"

// #define RELEASE

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// As of 0.3: 6080 lines of code
	// I have progressed past 0.3, so there is more than 6080 lines of code.
	// Keep this message as a tracker for LOC in versions.


	/*
	 * The TODOs for saucebot go here
	 * 
	 * -> Window classes
	 *     -> The container window--a window made to hold other windows
	 *     -> Why I never thought of this earlier I have no idea
	 *     -> Signals when mouse wheel
	 *     -> Full support for origin transformations (on the dc level)--when origin is transformed, signal children.
	 * 
	 * -> Download manager (lsdownload command)
	 *     -> Fairly self-explanatory--can delete, view, whatever
	 * 
	 * -> Search command
	 *     -> Ideas?
	 * 
	 * -> 0.4
	 *     -> Need a name for it
	 *     -> Should have all the specified improvements/commands above.
	 *     -> Start telling people about it outsite of 4/g/ once this version is done.
	 *     -> When this is finished, the app should be superior to a browser (mostly).
	 */
	
	#ifdef RELEASE
	FreeConsole( );
	#endif

	initGUI( );
	initSB( );
	
	// Master window
	sbWnd_Layout lout = { 0 };
	lout.dims.intDims[0] = CW_USEDEFAULT;
	lout.dims.intDims[1] = CW_USEDEFAULT;
	lout.dims.floatDims[2] = 0.5f;
	lout.dims.floatDims[3] = 0.5f;
	lout.type = SB_DIMTYPE_IIFF_TL_SRNN;

	sbWnd *first = SBMasterWindows.create( L"Saucebot 0.3 \"エッチ\"" , &lout );

	// Textbox
	lout.parent = first;
	lout.type = SB_DIMTYPE_FFFI_TL_SRNN;
	lout.dims.floatDims[0] = 0.15f;
	lout.dims.floatDims[1] = SB_CENTER_DIM( 0.925f );
	lout.dims.floatDims[2] = 0.55f;
	lout.dims.intDims[3] = 19;

	sbWnd *second = SBTextboxes.create( L"tbox-1", &lout, 0 );

	// Logger
	lout.type = SB_DIMTYPE_IIFF_TR_SRNN;
	lout.dims.intDims[0] = 1;
	lout.dims.intDims[1] = 0;
	lout.dims.floatDims[2] = 0.25f;
	lout.dims.floatDims[3] = 1.0f;

	sbWnd *third = SBBasicTextWindows.create( L"twnd-1", &lout, 15 );
	
	// Viewer
	lout.type = SB_DIMTYPE_IIFF_TL_SRNN;
	lout.dims.floatDims[2] = 0.75f;
	lout.dims.floatDims[3] = 0.85f;
	
	sbWnd *fourth = SBBasicTextWindows.create( L"basicwnd-1", &lout, 15 );

	SBWindows.setSignalFn( second, sbEntry );
	SBWindows.appendReference( second, fourth );
	SBWindows.appendReference( second, third );
	SBBasicTextWindows.draw( fourth, L"Type a command to get started!" );
	SBBasicTextWindows.draw( fourth, L"Alternatively, type 'info' to see what you can do." );
	SetFocus( GetHWND( second ) );

	msgloop( );
	shutdownSB2( );
	shutdownSB( );
	return EXIT_SUCCESS;
}
