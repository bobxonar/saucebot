#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <string.h>
#include <process.h>

#include "sbwnd/sbgui.h"
#include "sb2/sb2.h"

#define RELEASE

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// As of 0.3: 6064 lines of code


	/*
	 * The TODOs for saucebot go here
	 * -> Integrate number and download commands
	 *     -> The number command should be aware of what is already downloaded and display those
	 *     -> Add a 'downloads' option to the number command
	 *         -> It should bring up a scrollable list of numbers you have downloaded
	 *            they should have titles as well. Need to give downloads an info file.
	 * 
	 * -> General UI improvements
	 *     -> Fix unnecessary flickering in images - stop redrawing windows so often.
	 *     -> Fix basic text window performance - make one DrawTextEx call instead of several per paint message.
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
