#ifndef SBWND_PROC_MODULE
#define SBWND_PROC_MODULE

#include <windows.h>

#include "sbwnd_general.h"

LRESULT CALLBACK BasicWndProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK TextboxProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK TextWndProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK ClickableProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK RestrictedImageProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK MasterProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK ViewcmdMasterProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK StringProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK ProgressBarProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK DldcmdMasterProc( HWND, UINT, WPARAM, LPARAM );

#define SBM_DRAWTEXT    (WM_APP+0x01)
#define SBM_CLEARTEXT   (WM_APP+0x02)
#define SBM_UPDATEIMAGE (WM_APP+0x03)

void UpdateTiedown( SBTextboxInfo *, wchar_t );
void UpdateTextboxInfo( sbWnd * );
void ResetTextboxInfo( SBTextboxInfo * );

void DoubleWStringCapacity( SBTextboxInfo * );

/* Sizing */
BOOL CALLBACK ChildSizingProc( IN HWND, IN LPARAM );

/* Changing font */
BOOL CALLBACK ChildFontChangeProc( IN HWND, IN LPARAM );

/* Child window deletion */
BOOL CALLBACK ChildDestroyProc( IN HWND, IN LPARAM );

#endif