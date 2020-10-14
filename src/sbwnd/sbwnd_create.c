#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <gdiplus.h>

#include "sb2/sb_ds.h"
#include "sbwnd/sbwnd_init.h"
#include "sbwnd/sbwnd_create.h"
#include "sbwnd/sbwnd_general.h"

sbWnd *CommonInfoCreatorLayout( const wString name, sbWnd_Layout *lout ) {
	sbWnd *fin = newptr( sbWnd );
	if ( fin == NULL )
		return NULL;

	fin->id = SbGUIMaster.large_id++;
	fin->referenceList = Lists.New( );
	fin->parent = GetHWND( lout->parent );
	fin->name = name;
	fin->lout = newptr( sbWnd_Layout );
	fin->parentSb = lout->parent;
	fin->sibling = lout->sibling;

	if ( fin->lout == NULL )
		return NULL;
	memcpy( fin->lout, lout, sizeof( sbWnd_Layout ) );
	if ( lout->parent == NULL && wcscmp( name, L"testwindow" ) ) // Test window doesn't recieve messages.
		Lists.Add( SbGUIMaster.TopLevelWindows, fin );
	return fin;
}


int sbWndEvaluateLayout( sbWnd_Layout *lout, dimension evalDims[4] ) {
	sbWnd_Dims *dims = &lout->dims;
	uint8_t dimType = lout->type;

	// Parent rectangle width and height.
	int
		pw = SbGUIMaster.scrWidth,
		ph = SbGUIMaster.scrHeight;

	if ( lout->parent != NULL ) {

		RECT r = { 0 };
		/*
		 * I'd rather not have to use GetClientRect() here,
		 * but this does not work otherwise. I don't know how
		 * exactly it's implemented, but memcpy() on a saucebot-maintained
		 * client rectangle doesn't work in the case of maximizations.
		 */
		GetClientRect( GetHWND( lout->parent ), &r );
		pw = r.right;
		ph = r.bottom;

	}


	// Next, evaluate width, height, and inital evaluation for corners.
	// This is the original float to int conversion--essentially the subRect()
	// function from earlier versions of saucebot.
	// Even then, this has more functionality than subRect() anyways :^).
	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			evalDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? pw : ph );
		else
			evalDims[i] = dims->intDims[i];
	}

	// Next, the corners.
	// There are 2 possible tracks for this: sibling relative and parent relative.
	// Parent relative is the only evaluation method availible right now,
	// the largest advantage of these layouts is the sibling relative positioning option.

	dimension
		cornerX = evalDims[0],
		cornerY = evalDims[1];
	
	if ( lout->sibling != NULL )
		goto sibling_track;

	// Evaluate centering to top left for now.
	if ( cornerX < 0 && cornerX != CW_USEDEFAULT )
		cornerX = ( -cornerX ) - ( evalDims[2] / 2 );

	if ( cornerY < 0 && cornerY != CW_USEDEFAULT )
		cornerY = ( -cornerY ) - ( evalDims[3] / 2 );

	
	// Evaluate transformed corners.
	if ( dimType & 0x10 )
		cornerX = ( pw - cornerX ) - evalDims[2];
	
	if ( dimType & 0x20 )
		cornerY = ( ph - cornerY ) - evalDims[3];


	evalDims[0] = cornerX;
	evalDims[1] = cornerY;

	evalDims[0] += lout->offset[0];
	evalDims[1] += lout->offset[1];

	return 0;

	sibling_track: (void)0; // fuck off gcc I want to use my own variable declaration style

	if ( cornerX == CW_USEDEFAULT || cornerY == CW_USEDEFAULT )
		return 1; // CW_USEDEFAULT is not to be used in sibling-relative positioning

	dimension
		sCornerX,
		sCornerY,
		sDims[4] = { 0 };
	
	int res = sbWndEvaluateLayout( lout->sibling->lout, sDims );
	if ( res )
		return 1;

	sCornerX = 0x40 & dimType // sibling corner left/right
	?	sDims[0] + sDims[2]
	:	sDims[0];

	sCornerY = 0x80 & dimType // sibling corner top/bottom
	?	sDims[1] + sDims[3]
	:	sDims[1];

	// Centering ignores sibling corner and just goes to the center.
	// Note: you currently cannot center at 0. Just a thought...
	// Also note: positioning windows relative to the center on Y and X
	// can be used for radial positioning. It has no use for saucebot,
	// just thought I'd put that out there for any future readers.
	if ( cornerX < 0 ) {
		sCornerX = sDims[0] + sDims[2] / 2;
		cornerX = -cornerX;
	}

	if ( cornerY < 0 ) {
		sCornerY = sDims[1] + sDims[3] / 2;
		cornerY = -cornerY;
	}

	// Evaluate window corner, now that the sibling corner has been found.
	// Evaluate to top left first, then move to other corner.
	cornerX = sCornerX + ( 0x40 & dimType ? 1 : -1 ) * cornerX;
	cornerX -= 0x10 & dimType ? evalDims[2] : 0;

	cornerY = sCornerY + ( 0x80 & dimType ? 1 : -1 ) * cornerY;
	cornerY -= 0x20 & dimType ? evalDims[3] : 0;
	
	evalDims[0] = cornerX;
	evalDims[1] = cornerY;

	evalDims[0] += lout->offset[0];
	evalDims[1] += lout->offset[1];

	return 0;
}

HWND HWNDCreator( const wString name, sbWnd_Layout *lout, DWORD style, DWORD exs, const wString cname, sbWnd *wnd ) {
	dimension finDims[4] = { 0 };
	int res = sbWndEvaluateLayout( lout, finDims );
	if ( res )
		return NULL;

	return CreateWindowExW(
		exs,
		cname,
		name,
		style,
		finDims[0],
		finDims[1],
		finDims[2],
		finDims[3],
		GetHWND( lout->parent ),
		NULL,
		GetModuleHandleW( NULL ),
		wnd
	);
}

HWND BasicWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD | WS_BORDER;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ BASIC_WINDOW ], wnd
	);
}

HWND TextboxHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {
	// Textboxes MUST have a parent window.
	if ( lout->parent == NULL ) return NULL;
	
	DWORD style = WS_BORDER | WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;
	
	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ TEXTBOX_WINDOW ], wnd
	);
}

HWND BasicTextWindowHWNDCreator( const wString name, sbWnd_Layout *lout , sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD | WS_BORDER;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;
	
	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ TEXT_WINDOW ], wnd
	);

}

HWND ClickableWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {
	// Clickable windows MUST have a parent window.
	if ( lout->parent == NULL ) return NULL;
	
	DWORD style = WS_BORDER | WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;
	
	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ CLICKABLE_WINDOW ], wnd
	);
}

HWND RestrictedImageWindowHWNDCreator( const wString name, sbWnd_Layout *lout , sbWnd *wnd ) {
	
	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ RESTRICTED_IMAGE_WINDOW ], wnd
	);
}

HWND MasterWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {
	if ( lout->parent != NULL )
		return NULL; // Master windows are not allowed to have a parent.

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;
	
	return HWNDCreator(
		name, lout, style, WS_EX_COMPOSITED,
		SbGUIMaster.WindowClassNameArray[ MASTER_WINDOW ], wnd
 	);
}

HWND ViewcmdMasterWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ], wnd
	);
}

HWND StringWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ STRING_WINDOW ], wnd
	);
}

HWND ProgressBarWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {
	// Progress bars MUST have a parent window.
	if ( lout->parent == NULL ) return NULL;

	DWORD style = WS_CHILD | WS_BORDER;
	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;
	
	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ PROGRESS_BAR_WINDOW ], wnd
	);
}

HWND DldcmdMasterWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {

	DWORD style = WS_OVERLAPPEDWINDOW;
	if ( lout->parent != NULL )
		style = WS_CHILD;

	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ DLDCMD_MASTER_WINDOW ], wnd
	);
}

HWND VScrollbarWindowHWNDCreator( const wString name, sbWnd_Layout *lout, sbWnd *wnd ) {
	// VScrollbar widows MUST have a parent window.
	if ( lout->parent == NULL )
		return NULL;

	DWORD style = WS_CHILD;
	if ( SbGUIMaster.createMode )
		style |= WS_VISIBLE;

	return HWNDCreator(
		name, lout, style, 0,
		SbGUIMaster.WindowClassNameArray[ VSCROLLBAR_WINDOW ], wnd
	);
}

sbWnd *BasicWindowCreator( const wString name, sbWnd_Layout *lout ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = BasicWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = BASIC_WINDOW;

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *TextboxCreator( const wString name, sbWnd_Layout *lout, int enterAction ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = TextboxHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = TEXTBOX_WINDOW;
	fin->specific = TextboxInfoCreator( fin->hwnd, enterAction );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *BasicTextWindowCreator( const wString name, sbWnd_Layout *lout, uint16_t nLines ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = BasicTextWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = TEXT_WINDOW;
	fin->specific = BasicTextWindowInfoCreator( fin->hwnd, nLines );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *ClickableWindowCreator( const wString name, sbWnd_Layout *lout ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = ClickableWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = CLICKABLE_WINDOW;
	fin->specific = ClickableWindowInfoCreator( );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *RestrictedImageWindowCreator( const wString name, sbWnd_Layout *lout, const wString path ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = RestrictedImageWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = RESTRICTED_IMAGE_WINDOW;
	fin->specific = RestrictedImageWindowInfoCreator( fin->hwnd, path );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *MasterWindowCreator( const wString name, sbWnd_Layout *lout ) {
	if ( SbGUIMaster.masterWnd != NULL )
		return NULL;

	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = MasterWindowHWNDCreator( name, lout, fin );

	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = MASTER_WINDOW;
	SbGUIMaster.masterWnd = fin;

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *ViewcmdMasterWindowCreator( const wString name, sbWnd_Layout *lout ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = ViewcmdMasterWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = VIEWCMD_MASTER_WINDOW;

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *StringWindowCreator( const wString name, sbWnd_Layout *lout, const wString str, uint16_t fontSize, int clickable ) {
	// Self-sizing.
	sbWnd_Layout selfSize = { 0 };
	memcpy( &selfSize, lout, sizeof( sbWnd_Layout ) );
	selfSize.dims.intDims[2] = SBWindows.getStringWidth( str, fontSize ) + 4;
	selfSize.dims.intDims[3] = fontSize + 2;
	selfSize.type &= 0x3C;

	sbWnd *fin = CommonInfoCreatorLayout( name, &selfSize );
	fin->type = STRING_WINDOW;
	fin->hwnd = StringWindowHWNDCreator( name, &selfSize, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->specific = StringWindowInfoCreator( str, fontSize, clickable );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *DldcmdMasterWindowCreator( const wString name, sbWnd_Layout *lout ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = DldcmdMasterWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = DLDCMD_MASTER_WINDOW;

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *ProgressBarWindowCreator( const wString name, sbWnd_Layout *lout, uint16_t total ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = ProgressBarWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = PROGRESS_BAR_WINDOW;
	fin->specific = ProgressBarWindowInfoCreator( total );

	UpdateWindow( fin->hwnd );
	return fin;
}

sbWnd *VScrollbarWindowCreator( const wString name, sbWnd_Layout *lout, uint16_t incDist ) {
	sbWnd *fin = CommonInfoCreatorLayout( name, lout );
	fin->hwnd = VScrollbarWindowHWNDCreator( name, lout, fin );
	
	if ( fin->hwnd == NULL )
		return NULL;

	fin->type = VSCROLLBAR_WINDOW;
	fin->specific = VScrollbarWindowInfoCreator( incDist );

	UpdateWindow( fin->hwnd );
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

	sbWnd_Layout lout = { 0 };
	lout.parent = ( sbWnd * )GetWindowLongPtrW( hwnd, GWLP_USERDATA );
	lout.type = SB_DIMTYPE_IIIF_TR_SRNN;
	lout.dims.intDims[0] = 1;
	lout.dims.intDims[2] = 11;
	lout.dims.floatDims[3] = 1.0f;

	SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
	fin->scrollbar = SBVScrollbarWindows.create( L"vscrollbar-textwnd", &lout, 15 );
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

void destroy_AllTypes( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;

	switch( wnd->type ) {

		case TEXTBOX_WINDOW:
			sbWnd_DeleteTextboxInfo( wnd->specific );
			break;

		case TEXT_WINDOW:
			sbWnd_DeleteBasicTextWindowInfo( wnd->specific );
			break;

		case CLICKABLE_WINDOW:
			sbWnd_DeleteClickableWindowInfo( wnd->specific );
			break;

		case RESTRICTED_IMAGE_WINDOW:
			sbWnd_DeleteRestrictedImageWindowInfo( wnd->specific );
			break;

		case STRING_WINDOW:
			sbWnd_DeleteStringWindowInfo( wnd->specific );
			break;

		case PROGRESS_BAR_WINDOW:
			sbWnd_DeleteProgressBarWindowInfo( wnd->specific );
			break;

		case VSCROLLBAR_WINDOW:
			sbWnd_DeleteVScrollbarWindowInfo( wnd->specific );
			break;

	}
	sbWndCommonDestructor( wnd );
	return;
}

void sbWndCommonDestructor( sbWnd *wnd ) {
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->lout );
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