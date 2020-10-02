#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <gdiplus.h>

#include "sb2/sb_ds.h"
#include "sbwnd/sbwnd_init.h"
#include "sbwnd/sbwnd_create.h"
#include "sbwnd/sbwnd_general.h"

sbWnd *CommonInfoCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = newptr( sbWnd );
	fin->id = SbGUIMaster.large_id++;
	fin->referenceList = Lists.New( );
	fin->parent = parent;
	fin->name = name;
	fin->dimType = dimType;
	fin->dims = newptr( sbWnd_Dims );
	memcpy( fin->dims, dims, sizeof( sbWnd_Dims ) );
	if ( parent == NULL && wcscmp( name, L"testwindow" ) ) // Test window doesn't recieve messages.
		Lists.Add( SbGUIMaster.TopLevelWindows, fin );
	return fin;
}

void sbWndEvaluateDims( HWND parent, uint8_t dimType, sbWnd_Dims *dims, dimension evalDims[4] ) {
	int
		w = 0,
		h = 0;

	/* 
	dimension
		parentDims[4] = { 0 },
		siblingDims[4] = { 0 };

	*/

	// Evaluate parent dimensions--this will eventually become recursive (no it wont retard) as
	// dimension evaluation is a dynamic process as windows resize.
	if ( dimType & 0xF && parent == NULL ) {
	
		w = SbGUIMaster.scrWidth;
		h = SbGUIMaster.scrHeight;

	} else if ( parent != NULL ) {

		RECT r = { 0 };
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;

	}

	// Sibling evaluation goes here in the future.

	// Initial evaluation of dimensions--basic float to int conversion
	// on the lowest 4 bits of the dimension type.
	// This assumes top left cornering at first.
	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			evalDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			evalDims[i] = dims->intDims[i];
	}

	dimension
		cornerX = evalDims[0],
		cornerY = evalDims[1];

	// Evaluate centering to top left for now.
	if ( cornerX < 0 && cornerX != CW_USEDEFAULT )
		cornerX = ( -cornerX ) - ( evalDims[2] / 2 );

	if ( cornerY < 0 && cornerY != CW_USEDEFAULT )
		cornerY = ( -cornerY ) - ( evalDims[3] / 2 );

	
	// Evaluate transformed corners. Uses the correct transformations
	// this time.
	if ( dimType & 0x10 )
		cornerX = ( w - cornerX ) - evalDims[2];
	
	if ( dimType & 0x20 )
		cornerY = ( h - cornerY ) - evalDims[3];


	evalDims[0] = cornerX;
	evalDims[1] = cornerY;
	return;
}

HWND BasicWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD | WS_BORDER;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ BASIC_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND TextboxHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {
	// Textboxes MUST have a parent window.
	if ( parent == NULL ) return NULL;
	
	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ TEXTBOX_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND BasicTextWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD | WS_BORDER;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ TEXT_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);

}

HWND ClickableWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {
	// Clickable windows MUST have a parent window.
	if ( parent == NULL ) return NULL;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ CLICKABLE_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND RestrictedImageWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {
	
	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ RESTRICTED_IMAGE_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND MasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {
	if ( parent != NULL )
		return NULL; // Master windows are not allowed to have a parent.

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		WS_EX_COMPOSITED,
		SbGUIMaster.WindowClassNameArray[ MASTER_WINDOW ],
		name,
		WS_OVERLAPPEDWINDOW,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND ViewcmdMasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND StringWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ STRING_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND ProgressBarWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {
	// Progress bars MUST have a parent window.
	if ( parent == NULL ) return NULL;
	
	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ PROGRESS_BAR_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND DldcmdMasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND VScrollbarWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( parent != NULL )
		style = WS_CHILD;

	dimension finDims[4] = { 0 };
	sbWndEvaluateDims( parent, dimType, dims, finDims );
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ VSCROLLBAR_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

sbWnd *BasicWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = BasicWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = BASIC_WINDOW;
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *TextboxCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, int enterAction ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = TextboxHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = TEXTBOX_WINDOW;
	fin->specific = TextboxInfoCreator( fin->hwnd, enterAction );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *BasicTextWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, uint16_t nLines ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = BasicTextWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = TEXT_WINDOW;
	fin->specific = BasicTextWindowInfoCreator( fin->hwnd, nLines );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *ClickableWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ClickableWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = CLICKABLE_WINDOW;
	fin->specific = ClickableWindowInfoCreator( );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *RestrictedImageWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, const wString path ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = RestrictedImageWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = RESTRICTED_IMAGE_WINDOW;
	fin->specific = RestrictedImageWindowInfoCreator( fin->hwnd, path );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *MasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	if ( SbGUIMaster.masterWnd != NULL )
		return NULL;

	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = MasterWindowHWNDCreator( parent, name, dimType, dims, fin );

	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = MASTER_WINDOW;
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	SbGUIMaster.masterWnd = fin;
	return fin;
}

sbWnd *ViewcmdMasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ViewcmdMasterWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = VIEWCMD_MASTER_WINDOW;
	if ( SbGUIMaster.createMode ) {
		ShowWindow( fin->hwnd, SW_SHOW );
		UpdateWindow( fin->hwnd );
	}
	return fin;
}

sbWnd *StringWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, const wString str, uint16_t fontSize, int clickable ) {
	// Self-sizing.
	sbWnd_Dims selfSize = { 0 };
	memcpy( &selfSize, dims, sizeof( sbWnd_Dims ) );
	selfSize.intDims[2] = SBWindows.getStringWidth( str, fontSize ) + 4;
	selfSize.intDims[3] = fontSize + 2;

	sbWnd *fin = CommonInfoCreator( parent, name, ( dimType & 0x3C ), &selfSize );
	fin->hwnd = StringWindowHWNDCreator( parent, name, ( dimType & 0x3C ), &selfSize, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->specific = StringWindowInfoCreator( str, fontSize, clickable );
	fin->type = STRING_WINDOW;
	if ( SbGUIMaster.createMode ) {
		ShowWindow( fin->hwnd, SW_SHOW );
		UpdateWindow( fin->hwnd );
	}
	return fin;
}

sbWnd *DldcmdMasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = DldcmdMasterWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = DLDCMD_MASTER_WINDOW;
	if ( SbGUIMaster.createMode ) {
		ShowWindow( fin->hwnd, SW_SHOW );
		UpdateWindow( fin->hwnd );
	}
	return fin;
}

sbWnd *ProgressBarWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, uint16_t total ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ProgressBarWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = PROGRESS_BAR_WINDOW;
	fin->specific = ProgressBarWindowInfoCreator( total );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *VScrollbarWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, uint16_t incDist ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = VScrollbarWindowHWNDCreator( parent, name, dimType, dims, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = VSCROLLBAR_WINDOW;
	fin->specific = VScrollbarWindowInfoCreator( incDist );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

SBTextboxInfo *TextboxInfoCreator( HWND hwnd, int enterAction ) {

	SBTextboxInfo *fin = newptr( SBTextboxInfo );
	fin->tiedown = newwstr( 10 );
	fin->tiedownCapacity = 10;
	fin->enterAction = enterAction;
	RECT *r = newptr( RECT );
	GetClientRect( hwnd, r );

	++r->top;
	--r->bottom;
	++r->left;
	--r->right;
	fin->tbRect = r;

	int h = r->bottom - r->top;
	fin->currentFont = newptr( LOGFONTW );
	memcpy( fin->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
	fin->currentFont->lfHeight = h;
	fin->currentCaretPosition = 2;
	fin->canDisplay = 1;
	return fin;

}

SBBasicTextWindowInfo *BasicTextWindowInfoCreator( HWND hwnd, uint16_t fontSize ) {
	SBBasicTextWindowInfo *fin = newptr( SBBasicTextWindowInfo );
	fin->text = Lists.New( );
	fin->currentFont = newptr( LOGFONTW );
	memcpy( fin->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
	fin->fontSize = fontSize;

	RECT r = { 0 };
	GetClientRect( hwnd, &r );

	fin->currentMaxLines = r.bottom / fontSize;
	fin->prevMaxLines = fin->currentMaxLines;

	int h = r.bottom - r.top;
	fin->currentFont->lfHeight = fin->fontSize;

	sbWnd_Dims dims = { 0 };
	dims.intDims[0] = 1;
	dims.intDims[2] = 11;
	dims.floatDims[3] = 1.0f;
	SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
	fin->scrollbar = SBVScrollbarWindows.create(
		hwnd,
		L"vscrollbar-textwnd",
		SB_DIMTYPE_IIIF_TR,
		&dims,
		15
	);
	SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );

	return fin;
}

SBClickableWindowInfo *ClickableWindowInfoCreator( void ) {
	SBClickableWindowInfo *fin = newptr( SBClickableWindowInfo );
	fin->currentFont = SbGUIMaster.currentFont;
	return fin;
}

SBRestrictedImageWindowInfo *RestrictedImageWindowInfoCreator( HWND hwnd, const wString path ) {
	SBRestrictedImageWindowInfo *fin = newptr( SBRestrictedImageWindowInfo );

	if ( path == NULL ) {
		fin->img = NULL;
		fin->path = wcsdup( L"none" );
		return fin;
	}

	GpImage *img;
	GdipLoadImageFromFile( path, &img );
	GdipGetImageWidth( img, &fin->maxWidth );
	GdipGetImageHeight( img, &fin->maxHeight );
	fin->img = img;

	fin->aspectRatio = ( double )fin->maxWidth / ( double )fin->maxHeight;
	fin->path = wcsdup( path );
	return fin;
}

SBStringWindowInfo *StringWindowInfoCreator( const wString str, uint16_t fontSize, int clickable ) {
	SBStringWindowInfo *fin = newptr( SBStringWindowInfo );

	fin->currentFont = newptr( LOGFONTW );
	memcpy( fin->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );

	fin->clickable = clickable;
	fin->fontSize = fontSize;
	fin->currentFont->lfHeight = fontSize;
	fin->currentFont->lfWidth = 0;

	fin->str = wcsdup( str );
	return fin;
}

SBProgressBarWindowInfo *ProgressBarWindowInfoCreator( uint16_t total ) {
	SBProgressBarWindowInfo *fin = newptr( SBProgressBarWindowInfo );
	fin->total = total;
	return fin;
}

SBVScrollbarWindowInfo *VScrollbarWindowInfoCreator( uint16_t incDist ) {
	SBVScrollbarWindowInfo *fin = newptr( SBVScrollbarWindowInfo );
	fin->incDist = incDist;
	fin->maxInc = 1;
	fin->cur = 0;
	return fin;
}

void destroy_SbBasicWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;

	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbTextbox( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;

	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteTextboxInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbBasicTextWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteBasicTextWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbClickableWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteClickableWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbRestrictedImageWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteRestrictedImageWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbViewcmdMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbStringWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteStringWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbProgressBarWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteProgressBarWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbDldcmdMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbVScrollbarWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteVScrollbarWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void sbWnd_DeleteTextboxInfo( SBTextboxInfo *info ) {
	free( info->currentFont );
	free( info->tbRect );
	free( info->tiedown );
	free( info );
}

void sbWnd_DeleteBasicTextWindowInfo( SBBasicTextWindowInfo *info ) {
	Lists.Delete( info->text );
	free( info->currentFont );
	free( info );
}

void sbWnd_DeleteClickableWindowInfo( SBClickableWindowInfo *info ) {
	free( info->currentFont );
	free( info );
}

void sbWnd_DeleteRestrictedImageWindowInfo( SBRestrictedImageWindowInfo *info ) {
	if ( info->img != NULL )
		GdipDisposeImage( info->img );
	free( info->path );
	free( info );
}

void sbWnd_DeleteStringWindowInfo( SBStringWindowInfo *info ) {
	free( info->currentFont );
	free( info );
}

void sbWnd_DeleteProgressBarWindowInfo( SBProgressBarWindowInfo *info ) {
	free( info );
}

void sbWnd_DeleteVScrollbarWindowInfo( SBVScrollbarWindowInfo *info ) {
	free( info );
}