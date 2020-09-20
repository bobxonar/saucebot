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
	 * -> General UI improvements
	 *     -> Semantic change: change in/out functions to a single signal function.
	 *     -> Fix unnecessary flickering in images - stop redrawing windows so often.
	 *     -> Fix basic text window performance - make one DrawTextEx call instead of several per paint message.
	 * 
	 * -> New window classes
	 *     -> The container window--a window made to hold other windows
	 *     -> Why I never thought of this earlier I have no idea
	 *     -> Signals when mouse wheel
	 * 
	 * -> The dimensioning system
	 *     -> Make a seperate dimension evaluator function
	 *     -> Add sibling-relative positioning
	 *     -> Make a dimension struct instead of two variables
	 *     -> Add integer offsets and relative window to this struct
	 *     -> Corner specifications should evaluate to the cornet of the existing window, not the window to be created.
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
	 *     -> Start telling people about it outsite of 4ch once this version is done.
	 *     -> When this is finished, the app should be superior to a browser (mostly).
	 */
	
	#ifdef RELEASE
	FreeConsole( );
	#endif

	initGUI( );
	initSB( );
	
	sbWnd_Dims dims = { .intDims = { SB_CENTER_DIM( 960 ), SB_CENTER_DIM( 540 ), 960, 540 } };
	sbWnd *first = SBMasterWindows.create( NULL, L"Saucebot 0.3 \"エッチ\"" , SB_DIMTYPE_IIII_TL, &dims );

	sbWnd_Dims tBoxDims = { .floatDims = { 0.15f, SB_CENTER_DIM( 0.925f ), 0.55f, 0.0f } };
	tBoxDims.intDims[3] = 19;
	sbWnd *second = SBTextboxes.create( GetHWND( first ), L"tbox-1", SB_DIMTYPE_FFFI_TL, &tBoxDims, 0 );

	sbWnd_Dims tWndDims = { .floatDims[0] = 0.75f, .floatDims[1] = 0.0f, .floatDims[2] = 0.25f, .floatDims[3] = 1.0f };
	sbWnd *third = SBBasicTextWindows.create( GetHWND( first ), L"twnd-1", SB_DIMTYPE_FFFF_TL, &tWndDims, 15 );
	
	sbWnd_Dims moreDims = { .floatDims = { 0.0f, 0.0f, 0.75f, 0.85f } };
	moreDims.intDims[0] = 0;
	moreDims.intDims[1] = 0;
	sbWnd *fourth = SBBasicTextWindows.create( GetHWND( first ), L"basicwnd-1", SB_DIMTYPE_IIFF_TL, &moreDims, 15 );

	SBWindows.setOut( second, sbEntry );
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
