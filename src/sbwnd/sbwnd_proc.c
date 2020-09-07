#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <gdiplus.h>

#include "sbwnd/sbwnd_proc.h"
#include "sbwnd/sbwnd_create.h"
#include "sbwnd/sbwnd_general.h"
#include "sbwnd/sbwnd_init.h"
#include "res/sbwnd_res.h"

#include "sb2/sb_proc.h"

// Checks a window has the current keyboard focus. If not, sends the message where it needs to go.
#define CHK_FOCUS_SEND_MSG(hwnd, msg, lParam, wParam)       if(GetFocus()!=hwnd){SendMessage(GetFocus(),msg,lParam,wParam);return 0;}(void)0

LRESULT CALLBACK BasicWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_LBUTTONDOWN:
			SetFocus( hwnd );
			break;

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			( void )0; // Do nothing. BeginPaint() will remove the update region so no further WM_PAINT messages are sent.
			EndPaint( hwnd, &ps );
			break;
		}

		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;

		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK TextboxProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_LBUTTONDOWN:
			SetFocus( hwnd );
			break;

		case WM_SETFOCUS: {
			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBTextboxInfo *info = GetSpecificHandle( wnd );

			RECT r = *info->tbRect;
			int hgt = r.bottom - r.top;

			CreateCaret( hwnd, NULL, 0, hgt );
			SetCaretPos( info->currentCaretPosition, 1 );
			ShowCaret( hwnd );

			break;
		}

		case WM_KILLFOCUS:
			DestroyCaret( );
			break;

		case WM_COMMAND: {
			sbWnd *sbw = Maps.Search(SbGUIMaster.WindowMap, hwnd);
			SBTextboxInfo *info = GetSpecificHandle( sbw );
			CHK_FOCUS_SEND_MSG( hwnd, msg, wParam, lParam );
			switch( LOWORD( wParam ) ) {
				case LARGE_BACK: {
					do
						SendMessage( hwnd, WM_CHAR, VK_BACK, 0 );
					while ( info->tiedownLength > 0 && info->tiedown[ info->tiedownLength - 1 ] != ' ');
					break;
				}
				case PASTE: {
					OpenClipboard( NULL );
					wString s = ( wString )GetClipboardData( CF_UNICODETEXT );
					for ( size_t i = 0; i < wcslen( s ); i++ )
						SendMessage( hwnd, WM_CHAR, s[i], 0 );
					CloseClipboard( );
					break;
				}
			}
			break;
		}

		case WM_SIZE: {
			UpdateWindow( hwnd );
			break;
		}

		case SBM_SIZE: {

			if ( GetFocus( ) == hwnd )
				DestroyCaret( );

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBTextboxInfo *info = GetSpecificHandle( wnd );
			UpdateTextboxInfo( wnd );

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );

			if ( GetFocus( ) == hwnd ) {
				CreateCaret( hwnd, NULL, 0, info->currentFont->lfHeight );
				SetCaretPos( info->currentCaretPosition, 1 );
				ShowCaret( hwnd );
			}

			break;
		}

		case SBM_CHANGEFONT: {

			if ( GetFocus( ) == hwnd )
				HideCaret( hwnd );

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBTextboxInfo *info = GetSpecificHandle( wnd );

			int h = info->currentFont->lfHeight;
			memcpy( info->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
			info->currentFont->lfHeight = h;

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );
			if ( GetFocus( ) == hwnd )
				ShowCaret( hwnd );

			break;
		}
		case WM_CHAR: {

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBTextboxInfo *info = GetSpecificHandle( wnd );

			if ( wParam == VK_RETURN && info->enterAction )
				break;

			HideCaret( hwnd );

			wString str = wcsdup( info->tiedown );
			uint8_t send = 0;

			switch( wParam ) {

				case VK_RETURN:

					ResetTextboxInfo( info );
					send = 1;

					break;

				case VK_BACK:

					if ( info->currentCaretPosition <= 2 )
						break;

					info->tiedown[ --info->tiedownLength ] = 0;

					break;

				default:

					if ( info->canDisplay )
						UpdateTiedown( info, ( wchar_t )wParam );

					break;

			}

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );
			SetCaretPos( info->currentCaretPosition, 1 );
			ShowCaret( hwnd );

			if ( send )
				SBWindows.out( wnd, wnd, str );
			else
				free( str );
			
			break;
		}

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {

			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );

			// This is the text rendering machine.

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBTextboxInfo *info = GetSpecificHandle( wnd );
			wString str = SBTextboxes.getString( wnd );
			LOGFONTW *logfont = info->currentFont;

			HBRUSH prevBrush = GetCurrentObject( dc, OBJ_BRUSH );
			HFONT prevFont = GetCurrentObject( dc, OBJ_FONT );
			HBRUSH brush = GetStockObject( BLACK_BRUSH );
			HFONT font = CreateFontIndirectW( logfont );

			SelectObject( dc, brush );
			SelectObject( dc, font );

			SIZE strSz = { 0 };
			DWORD szs = GetTabbedTextExtentW( dc, str, ( int )wcslen( str ), 0, NULL );
			strSz.cy = HIWORD( szs );
			strSz.cx = LOWORD( szs );
			if ( !( ( strSz.cx + 1 ) >= info->tbRect->right ) )
				info->currentCaretPosition = 2 + strSz.cx;
			else if ( info->canDisplay != 0 ) {
				info->tiedown[ --info->tiedownLength ] = '\0';
				info->canDisplay = 0;
			}

			TabbedTextOutW( dc, 2, 1, str, ( int )wcslen( str ), 0, NULL, 2 );

			SelectObject( dc, prevBrush );
			SelectObject( dc, prevFont );
			DeleteObject( brush );
			DeleteObject( font );

			EndPaint( hwnd, &ps );

			break;
		}

		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;

		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );

	}
	return 0;
}

LRESULT CALLBACK TextWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			
			// Rendering text. Now featuring wrapping!

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBBasicTextWindowInfo *info = GetSpecificHandle( wnd );
			sbList *text = info->text;
			int h = info->fontSize;

			HBRUSH brush = GetStockObject( BLACK_BRUSH );
			HFONT font = CreateFontIndirectW( info->currentFont );

			HBRUSH prevBrush = SelectObject( dc, brush );
			HFONT prevFont = SelectObject( dc, font );

			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			r.left++;

			for ( uint16_t i = 0; i < Lists.Size( text ); ++i ) {
				wString str = Lists.Get( text, i );
				DrawTextExW( dc, str, -1, &r, DT_EXPANDTABS | DT_WORDBREAK, NULL );

				DWORD sz = GetTabbedTextExtentW( dc, str, wcslen( str ), 0, NULL );
				uint16_t wrapl = LOWORD( sz ) / ( r.right - r.left );
				r.top += h + ( h * wrapl );
			}

			SelectObject( dc, prevBrush );
			SelectObject( dc, prevFont );
			DeleteObject( brush );
			DeleteObject( font );

			EndPaint( hwnd, &ps );
			break;
		}

		case WM_SIZE: {
			UpdateWindow( hwnd );
			EnumChildWindows( hwnd, ChildSizingProc, 0 );
			break;
		}

		case WM_LBUTTONDOWN: {
			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBWindows.out( wnd, wnd, NULL );
			SetFocus( hwnd );
			break;
		}
			
		case SBM_DRAWTEXT: {

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBBasicTextWindowInfo *info = GetSpecificHandle( wnd );
			Lists.Add( info->text, ( void * )wParam );

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );

			break;
		}

		case SBM_CLEARTEXT: {

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBBasicTextWindowInfo *info = GetSpecificHandle( wnd );
			Lists.Delete( info->text );
			info->text = Lists.New( );

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );

			break;
		}

		case SBM_CHANGEFONT: {

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBBasicTextWindowInfo *info = GetSpecificHandle( wnd );

			memcpy( info->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
			info->currentFont->lfHeight = info->fontSize;

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );

			break;
		}
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK ClickableProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			( void )0; // Do nothing
			EndPaint( hwnd, &ps );
			break;
		}

		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK RestrictedImageProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {

			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			uint32_t ih = r.bottom - r.top;
			uint32_t iw = r.right - r.left;

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBRestrictedImageWindowInfo *info = GetSpecificHandle( wnd );

			if ( !wcscmp( L"none", info->path ) )
				break;
			
			GpGraphics *gph;
			PAINTSTRUCT ps;

			HDC dc = BeginPaint( hwnd, &ps );

			GdipCreateFromHDC( dc, &gph );

			uint32_t w = iw > info->maxWidth ? info->maxWidth : iw;
			uint32_t h = ( uint32_t )( ( double )w / info->aspectRatio );

			if ( h > ih ) {
				h = ih;
				w = ( uint32_t )( ( double )h * info->aspectRatio );
			}

			int left = ( ( iw / 2 ) - ( w / 2 ) ) + r.left;
			int top = ( ( ih / 2 ) - ( h / 2 ) ) + r.top;
			GdipDrawImageRectI( gph, info->img, left, top, w, h );

			GdipDeleteGraphics( gph );
			EndPaint( hwnd, &ps );
			
			break;
		}
		case WM_SIZE:
			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );
			break;
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK MasterProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_LBUTTONDOWN:
			SetFocus( hwnd );
			break;

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			( void )0; // Do nothing.
			EndPaint( hwnd, &ps );
			break;
		}

		case WM_SIZE:
			EnumChildWindows( hwnd, ChildSizingProc, 0 );
			break;
		case WM_ACTIVATE:
			SetFocus( hwnd );
			break;
		case WM_COMMAND: {

			if ( HIWORD( wParam ) != 0 ) {
				CHK_FOCUS_SEND_MSG( hwnd, msg, wParam, lParam );
			}

			switch( LOWORD( wParam ) ) {

				case ITEM_FONTS: {

					CHOOSEFONTW chFont = { 0 };

					chFont.lStructSize = sizeof( CHOOSEFONT );
					chFont.lpLogFont = newptr( LOGFONTW );

					if ( ChooseFontW( &chFont ) == TRUE ) {
						memcpy( SbGUIMaster.currentFont, chFont.lpLogFont, sizeof( LOGFONTW ) );
						EnumChildWindows( hwnd, ChildFontChangeProc, 0 );
					}

					break;
				}
			}
			break;
		}
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 ); // THERE CAN ONLY BE ONE--DO NOT MAKE MORE THAN ONE MASTER WINDOW, ELSE SUFFER.
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK ViewcmdMasterProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_LBUTTONDOWN:
			SetFocus( hwnd );
			break;

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			( void )0; // Do nothing
			EndPaint( hwnd, &ps );
			break;
		}

		case WM_SIZE: {
			EnumChildWindows( hwnd, ChildSizingProc, 0 );
			break;
		}

		case WM_CHAR: {
			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			switch( wParam ) {
				case VK_LEFT:
					SBWindows.in( wnd, wnd, ( void * )SBEVT_PREVPAGE );
					break;
				case VK_RIGHT:
					SBWindows.in( wnd, wnd, ( void * )SBEVT_NEXTPAGE );
					break;
			}
			break;
		}
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK StringProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {

			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBStringWindowInfo *info = GetSpecificHandle( wnd );
			
			HFONT font = CreateFontIndirectW( info->currentFont );
			HBRUSH brush = GetStockObject( BLACK_BRUSH );

			HBRUSH prevBrush = SelectObject( dc, brush );
			HFONT prevFont = SelectObject( dc, font );

			TabbedTextOutW( dc, 1, 1, info->str, wcslen( info->str ), 0, NULL, 1 );

			SelectObject( dc, prevBrush );
			SelectObject( dc, prevFont );

			DeleteObject( font );
			DeleteObject( brush );

			EndPaint( hwnd, &ps );
			break;
		}

		case SBM_CHANGEFONT: {

			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBStringWindowInfo *info = GetSpecificHandle( wnd );

			memcpy( info->currentFont, SbGUIMaster.currentFont, sizeof( LOGFONTW ) );
			info->currentFont->lfHeight = info->fontSize;

			sbWnd_Dims newSize = { 0 };
			memcpy( &newSize, wnd->dims, sizeof( sbWnd_Dims ) );
			newSize.intDims[2] = SBWindows.getStringWidth( info->str, info->fontSize ) + 3;
			SBWindows.changeDims( wnd, wnd->dimType, &newSize );

			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );

			break;
		}

		case WM_LBUTTONDOWN: {
			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBWindows.out( wnd, wnd, NULL );
			break;
		}

		case WM_SIZE:
			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );
			break;
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT CALLBACK ProgressBarProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch ( msg ) {

		case WM_PAINT: {
			
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			
			sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
			SBProgressBarWindowInfo *info = GetSpecificHandle( wnd );
			RECT r = { 0 };
			GetClientRect( hwnd, &r );

			int w = r.right - r.left;
			r.right = r.left + ( w * info->cur / info->total );

			FillRect( dc, &r, GetStockObject( BLACK_BRUSH ) );

			EndPaint( hwnd, &ps );
			break;
		}

		case WM_SIZE:
			InvalidateRect( hwnd, NULL, TRUE );
			UpdateWindow( hwnd );
			break;
		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );

	}
	return 0;
}

LRESULT CALLBACK DldcmdMasterProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_LBUTTONDOWN:
			SetFocus( hwnd );
			break;

		case WM_ERASEBKGND: {
			HDC dc = ( void * )wParam;
			RECT r = { 0 };
			GetClientRect( hwnd, &r );
			FillRect( dc, &r, GetStockObject( WHITE_BRUSH ) );
			return 1;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps = { 0 };
			HDC dc = BeginPaint( hwnd, &ps );
			( void )0; // Do nothing
			EndPaint( hwnd, &ps );
			break;
		}

		case WM_SIZE: {
			EnumChildWindows( hwnd, ChildSizingProc, 0 );
			break;
		}

		case WM_CLOSE:
			DestroyWindow( hwnd );
			break;
		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}

int GetRectBottom( RECT *r ) {
	return r->bottom;
}

void UpdateTiedown( SBTextboxInfo *info, wchar_t val ) {
	if ( ( info->tiedownLength + 1 ) >=  info->tiedownCapacity )
		DoubleWStringCapacity( info );
	info->tiedown[ info->tiedownLength++ ] = val;
	return;
}

void UpdateTextboxInfo( sbWnd *wnd ) {
	if ( wnd->type != TEXTBOX_WINDOW ) return;
	SBTextboxInfo *info = GetSpecificHandle( wnd );

	RECT *r = newptr( RECT );
	GetClientRect( GetHWND( wnd ), r );
	++r->top;
	--r->bottom;
	++r->left;
	--r->right;
	info->tbRect = r;

	int h = r->bottom - r->top;
	info->currentFont->lfHeight = h;

	return;
}

void ResetTextboxInfo( SBTextboxInfo *info ) {
	info->currentCaretPosition = 2;
	info->tiedownCapacity = 10;
	info->tiedownLength = 0;
	info->canDisplay = 1;
	if ( wcslen( info->tiedown ) > 0 )
		free( info->tiedown );
	info->tiedown = newwstr( 10 );
	return;
}

void DoubleWStringCapacity( SBTextboxInfo *info ) {
	wString dblStr = newwstr( ( info->tiedownCapacity * 2 ) );
	wcscpy( dblStr, info->tiedown );
	free( info->tiedown );
	info->tiedownCapacity *= 2;
	info->tiedown = dblStr;
}

BOOL CALLBACK ChildSizingProc( IN HWND hwnd, IN LPARAM lParam ) {

	sbWnd *wnd = Maps.Search( SbGUIMaster.WindowMap, hwnd );
	RECT r = { 0 };
	dimension finDims[4] = { 0 };
	HWND parent = GetParent( hwnd );

	int w = 0, h = 0;
	if ( wnd->dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
	}

	sbWnd_Dims *dims = wnd->dims;
	for ( int i = 0; i < 4; ++i ) {
		if ( wnd->dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( wnd->dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( wnd->dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( wnd->dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( wnd->dimType & 0x20 )
		finDims[1] -= finDims[3];

	MoveWindow( hwnd, finDims[0], finDims[1], finDims[2], finDims[3], TRUE );
	InvalidateRect( hwnd, NULL, TRUE );
	UpdateWindow( hwnd );
	SendMessage( hwnd, SBM_SIZE, 0, 0 );
	return 1;
}

BOOL CALLBACK ChildFontChangeProc( IN HWND hwnd, IN LPARAM lParam) {
	SendMessageW( hwnd, SBM_CHANGEFONT, 0, 0 );
	return TRUE;
}

BOOL CALLBACK ChildZOrderProc( IN HWND hwnd, IN LPARAM lParam ) {
	BringWindowToTop( hwnd );
	InvalidateRect( hwnd, NULL, TRUE );
	UpdateWindow( hwnd );
	return TRUE;
}

BOOL CALLBACK ChildUpdateProc( IN HWND hwnd, IN LPARAM lParam ) {
	InvalidateRect( hwnd, NULL, TRUE );
	UpdateWindow( hwnd );
	return TRUE;
}

BOOL CALLBACK ChildHideProc( IN HWND hwnd, IN LPARAM lParam ) {
	ShowWindow( hwnd, SW_HIDE );
	return TRUE;
}

BOOL CALLBACK ChildShowProc( IN HWND hwnd, IN LPARAM lParam ) {
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
	return TRUE;
}

BOOL CALLBACK ChildDestroyProc( IN HWND hwnd, IN LPARAM lParam ) {
	DestroyWindow( hwnd );
	return TRUE;
}