#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <gdiplus.h>
#include <process.h>

#include "sb2/sb_ds.h"
#include "sbwnd/sbwnd_init.h"
#include "sbwnd/sbwnd_create.h"
#include "sbwnd/sbwnd_general.h"
#include "sbwnd/sbwnd_proc.h"

SBWindowFnStruct SBWindow;


HWND GetHWND( sbWnd *wnd ) { return wnd->hwnd; }

wString GetTextboxString( TBOX sbWnd *wnd ) {
	if ( wnd->type == TEXTBOX_WINDOW )
		return ( ( SBTextboxInfo * )wnd->specific )->tiedown;
	return NULL;
}

void *GetSpecificHandle( sbWnd *wnd ) { return wnd->specific; }

// bitch
void msgloop( void ) {

	HWND mwnd = GetHWND( SbGUIMaster.masterWnd );
	MSG msg = { 0 };
	while ( ( GetMessageW( &msg, mwnd, 0, 0 ) ) > 0 ) {
		if ( !TranslateAcceleratorW( mwnd , SbGUIMaster.sbAccelTable, &msg ) ) {
			TranslateMessage( &msg );
			DispatchMessageW( &msg );
		}
	}


	/* uint16_t sz = Lists.Size( SbGUIMaster.TopLevelWindows );
	while ( sz > 0 ) {
		for ( uint16_t i = 0; i < sz; ++i ) {
			sbWnd *wnd = Lists.Get( SbGUIMaster.TopLevelWindows, i );
			MSG m = { 0 };

			if ( !PeekMessageW( &m, wnd->hwnd, 0, 0, PM_NOREMOVE ) )
				continue;

			if ( GetMessageW( &m, wnd->hwnd, 0, 0 ) > 0 ) {
				if ( !TranslateAcceleratorW( wnd->hwnd , SbGUIMaster.sbAccelTable, &m ) ) {
					TranslateMessage( &m );
					DispatchMessageW( &m );
				}
			}

			if ( !IsWindow( wnd->hwnd ) ) {
				Lists.Remove( SbGUIMaster.TopLevelWindows, i );
				sz = Lists.Size( SbGUIMaster.TopLevelWindows );
			}

			if ( !IsWindow( GetHWND( SbGUIMaster.masterWnd ) ) )
				exit( EXIT_SUCCESS );
		}
	} */
	return;
}

void SetSignalFn_AllTypes( GENERICWND sbWnd *wnd, sbWndSignalFn func ) {
	wnd->signalFn = func;
}

void appendReference_AllTypes( GENERICWND sbWnd *wnd, void *refdata ) {
	Lists.Add( wnd->referenceList, refdata );
}

void *getReference_AllTypes( GENERICWND sbWnd *wnd, uint16_t idx ) {
	return Lists.Get( wnd->referenceList, idx );
}

uint32_t getStringWidth_AllTypes( wString str, uint32_t size ) {

	LOGFONTW font = { 0 };
	memcpy( &font, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
	font.lfHeight = size;
	font.lfWidth = 0;

	HFONT f = CreateFontIndirectW( &font );
	HDC dc = GetDC( GetHWND( SbGUIMaster.testWnd ) );
	HFONT prevFont = SelectObject( dc, f );
	DWORD sz = GetTabbedTextExtentW( dc, str, wcslen( str ), 0, NULL );
	

	SelectObject( dc, prevFont );
	DeleteObject( f );
	ReleaseDC( GetHWND( SbGUIMaster.testWnd ), dc );
	return LOWORD( sz );
}

void changeDims_AllTypes( GENERICWND sbWnd *wnd, uint8_t dimType, sbWnd_Dims *dims ) {
	wnd->dimType = dimType;
	memcpy( wnd->dims, dims, sizeof( sbWnd_Dims ) );
	ChildSizingProc( wnd->hwnd, 0 ); // Evaluates dimensions, moves the window, repaints the window.
}

void toSurface_AllTypes( GENERICWND sbWnd *wnd ) {
	BringWindowToTop( wnd->hwnd );
}

void focus_AllTypes( GENERICWND sbWnd *wnd ) {
	SetFocus( wnd->hwnd );
}

void getDims_AllTypes( GENERICWND sbWnd *wnd, uint8_t *dimType, sbWnd_Dims *dims ) {
	*dimType = wnd->dimType;
	memcpy( dims, wnd->dims, sizeof( sbWnd_Dims ) );
	return;
}

sbWnd *getParent_AllTypes( GENERICWND sbWnd *wnd ) {
	HWND phwnd = wnd->parent;
	if ( phwnd != NULL )
		return ( void * )GetWindowLongPtrW( wnd->parent, GWLP_USERDATA );

	return NULL;
}

void CallSignalFn_AllTypes( GENERICWND sbWnd *wnd, GENERICWND sbWnd *wnd_arg, void *data_arg ) {
	if ( wnd->signalFn != NULL )
		wnd->signalFn( wnd_arg, data_arg );
}

void draw_SbBasicTextWindow( BASICTXTWND sbWnd *wnd, wString str ) {
	if ( wnd->type != TEXT_WINDOW ) return;
	SendMessageW( GetHWND( wnd ), SBM_DRAWTEXT, ( WPARAM )str, 0 );
}

void clear_SbBasicTextWindow( BASICTXTWND sbWnd *wnd ) {
	if ( wnd->type != TEXT_WINDOW ) return;
	SendMessageW( GetHWND( wnd), SBM_CLEARTEXT, 0, 0 );
}

uint16_t getID_AllTypes( GENERICWND sbWnd *wnd ) {
	return wnd->id;
}

uint16_t getType_AllTypes( GENERICWND sbWnd *wnd ) {
	return wnd->type;
}

void setCreateMode_AllTypes( uint8_t type ) {
	SbGUIMaster.createMode = type;
}

void show_AllTypes( GENERICWND sbWnd *wnd ) {
	ShowWindow( wnd->hwnd, SW_SHOW );
}

void hide_AllTypes( GENERICWND sbWnd *wnd ) {
	ShowWindow( wnd->hwnd, SW_HIDE );
}

wString getString_SbTextbox( TBOX sbWnd *wnd ) {
	if ( wnd->type != TEXTBOX_WINDOW )
		return NULL;
	SBTextboxInfo *info = GetSpecificHandle( wnd );
	return wcsdup( info->tiedown );
}

void sendString_SbTextbox( TBOX sbWnd *wnd, wString str ) {
	if ( wnd->type != TEXTBOX_WINDOW || str == NULL )
		return;

	size_t len = wcslen( str );
	for ( size_t i = 0; i < len; ++i )
		SendMessage( GetHWND( wnd ), WM_CHAR, str[i], 0 );
}

void setEnterAction_SbTextbox( TBOX sbWnd *wnd, int enterAction ) {
	SBTextboxInfo *inf = GetSpecificHandle( wnd );
	inf->enterAction = enterAction;
}

int getEnterAction_SbTextbox( TBOX sbWnd *wnd ) {
	SBTextboxInfo *inf = GetSpecificHandle( wnd );
	return inf->enterAction;
}

void updateImage_SbRestrictedImageWindow( RSTIMGWND sbWnd *wnd, const wString path ) {
	if ( wnd->type != RESTRICTED_IMAGE_WINDOW )
		return;
	SBRestrictedImageWindowInfo *info = GetSpecificHandle( wnd );
	sbWnd_DeleteRestrictedImageWindowInfo( info );
	wnd->specific = RestrictedImageWindowInfoCreator( GetHWND( wnd ), path );
	InvalidateRect( GetHWND( wnd ), NULL, TRUE );
	UpdateWindow( GetHWND( wnd ) );
}

void changeString_SbStringWindow( STRWND sbWnd *wnd, const wString str ) {
	if ( wnd->type != STRING_WINDOW )
		return;
	SBStringWindowInfo *info = GetSpecificHandle( wnd );
	info->str = wcsdup( str );
	SendMessage( GetHWND( wnd ), SBM_CHANGEFONT, 0, 0 );
}

void advance_SbProgressBarWindow( PROGBARWND sbWnd *wnd ) {
	if ( wnd->type != PROGRESS_BAR_WINDOW )
		return;

	SBProgressBarWindowInfo *info = GetSpecificHandle( wnd );
	info->cur += info->cur < info->total ? 1 : 0;
	InvalidateRect( GetHWND( wnd ), NULL, FALSE );
	return;
}

void advance_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	info->cur += info->cur >= info->maxInc
	?	0
	:	1;
	InvalidateRect( wnd->hwnd, NULL, TRUE );
	return;
}

void retreat_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	info->cur -= info->cur <= 0
	?	0
	:	1;
	InvalidateRect( wnd->hwnd, NULL, TRUE );
	return;
}

void reset_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	info->cur = 0;

	InvalidateRect( wnd->hwnd, NULL, TRUE );
	return;
}

void setPos_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd, uint16_t pos ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	if ( pos > info->maxInc )
		return;

	info->cur = pos;
	InvalidateRect( wnd->hwnd, NULL, TRUE );
	return;
}

void setMaxIncrement_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd, uint16_t max ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	info->maxInc = max;
	if ( info->cur > max )
		info->cur = info->maxInc;
	InvalidateRect( wnd->hwnd, NULL, TRUE );
	return;
}

int getMaxIncrement_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return 0;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	return info->maxInc;
}

int getCurrentPos_SbVScrollbarWindow( VSCROLLWND sbWnd *wnd ) {
	if ( wnd->type != VSCROLLBAR_WINDOW )
		return 0;

	SBVScrollbarWindowInfo *info = GetSpecificHandle( wnd );
	return info->cur;
}