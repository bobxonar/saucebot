#ifndef SBWND_PROC_MODULE
#define SBWND_PROC_MODULE

#include <windows.h>

#include "sbwnd_general.h"

int sbWndCommonProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK
	BasicWndProc( HWND, UINT, WPARAM, LPARAM ),
	TextboxProc( HWND, UINT, WPARAM, LPARAM ),

	TextWndProc( HWND, UINT, WPARAM, LPARAM ),
	ClickableProc( HWND, UINT, WPARAM, LPARAM ),

	RestrictedImageProc( HWND, UINT, WPARAM, LPARAM ),
	MasterProc( HWND, UINT, WPARAM, LPARAM ),

	ViewcmdMasterProc( HWND, UINT, WPARAM, LPARAM ),
	StringProc( HWND, UINT, WPARAM, LPARAM ),

	ProgressBarProc( HWND, UINT, WPARAM, LPARAM ),
	DldcmdMasterProc( HWND, UINT, WPARAM, LPARAM ),

	VScrollbarProc( HWND, UINT, WPARAM, LPARAM );

#define SBM_DRAWTEXT    (WM_APP+0x01)
#define SBM_CLEARTEXT   (WM_APP+0x02)
#define SBM_UPDATEIMAGE (WM_APP+0x03)

void
	UpdateTiedown( SBTextboxInfo *, wchar_t ),
	UpdateTextboxInfo( sbWnd * ),
	ResetTextboxInfo( SBTextboxInfo * ),
	UpdateTextWndScrollbarInfo( SBBasicTextWindowInfo *, RECT * );

void DoubleWStringCapacity( SBTextboxInfo * );

/* Sizing */
BOOL CALLBACK ChildSizingProc( IN HWND, IN LPARAM );

/* Changing font */
BOOL CALLBACK ChildFontChangeProc( IN HWND, IN LPARAM );

/* Child window deletion */
BOOL CALLBACK ChildDestroyProc( IN HWND, IN LPARAM );

#endif