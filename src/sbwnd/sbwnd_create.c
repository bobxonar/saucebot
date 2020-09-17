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


HWND BasicWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD | WS_BORDER;
	}

	// Turn floats into ints using parent window dimensions.
	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	// Evaluate centered dimensions.
	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ BASIC_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND TextboxHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	// Textboxes MUST have a parent window.
	if ( parent == NULL ) return NULL;
	
	RECT r = { 0 };
	int w = 0, h = 0;
	dimension finDims[4] = { 0 };
	
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ TEXTBOX_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND BasicTextWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD | WS_BORDER;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ TEXT_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);

}

HWND ClickableWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	// Clickable windows MUST have a parent window.
	if ( parent == NULL ) return NULL;
	
	RECT r = { 0 };
	int w = 0, h = 0;
	dimension finDims[4] = { 0 };
	
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ CLICKABLE_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND RestrictedImageWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ RESTRICTED_IMAGE_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND MasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD | WS_BORDER;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		WS_EX_COMPOSITED,
		SbGUIMaster.WindowClassNameArray[ MASTER_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND ViewcmdMasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		WS_EX_TOPMOST,
		SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND StringWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ STRING_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND ProgressBarWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	// Progress bars MUST have a parent window.
	if ( parent == NULL ) return NULL;
	
	RECT r = { 0 };
	int w = 0, h = 0;
	dimension finDims[4] = { 0 };
	
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];
	
	return CreateWindowExW(
		0,
		SbGUIMaster.WindowClassNameArray[ PROGRESS_BAR_WINDOW ],
		name,
		WS_CHILD | WS_BORDER,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

HWND DldcmdMasterWindowHWNDCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	RECT r = { 0 };
	int w = 0, h = 0;
	DWORD style = WS_OVERLAPPEDWINDOW;

	dimension finDims[4] = { 0 };
	if ( dimType & 0xF && parent == NULL ) return NULL;

	// using scaling coefficients requires a parent window to exist.
	if ( dimType & 0xF ) {
		GetClientRect( parent, &r );
		w = r.right - r.left;
		h = r.bottom - r.top;
		style = WS_CHILD;
	}

	for ( int i = 0; i < 4; ++i ) {
		if ( dimType & ( 0x8 >> i ) )
			finDims[i] = dims->floatDims[i] * ( i % 2 == 0 ? w : h );
		else
			finDims[i] = dims->intDims[i];
	}

	if ( finDims[0] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x10 )
			finDims[0] = ( -finDims[0] ) + ( finDims[2] / 2 );
		else
			finDims[0] = ( -finDims[0] ) - ( finDims[2] / 2 );
	}
	if ( finDims[1] < 0 && finDims[0] != CW_USEDEFAULT ) {
		if ( dimType & 0x20 )
			finDims[1] = ( -finDims[1] ) + ( finDims[3] / 2 );
		else
			finDims[1] = ( -finDims[1] ) - ( finDims[3] / 2 );
	}

	if ( dimType & 0x10 )
		finDims[0] -= finDims[2];
	if ( dimType & 0x20 )
		finDims[1] -= finDims[3];

	return CreateWindowExW(
		WS_EX_TOPMOST,
		SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ],
		name,
		style,
		finDims[0], finDims[1], finDims[2], finDims[3],
		parent,
		NULL,
		GetModuleHandleW( NULL ),
		NULL
	);
}

sbWnd *BasicWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = BasicWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = BASIC_WINDOW;
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *TextboxCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, int enterAction ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = TextboxHWNDCreator( parent, name, dimType, dims );
	fin->type = TEXTBOX_WINDOW;
	fin->specific = TextboxInfoCreator( fin->hwnd, enterAction );
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *BasicTextWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, uint16_t nLines ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = BasicTextWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = TEXT_WINDOW;
	fin->specific = BasicTextWindowInfoCreator( fin->hwnd, nLines );
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *ClickableWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ClickableWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = CLICKABLE_WINDOW;
	fin->specific = ClickableWindowInfoCreator( );
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *RestrictedImageWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, const wString path ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = RestrictedImageWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = RESTRICTED_IMAGE_WINDOW;
	fin->specific = RestrictedImageWindowInfoCreator( fin->hwnd, path );
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	return fin;
}

sbWnd *MasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	if ( SbGUIMaster.masterWnd != NULL )
		return NULL;

	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = MasterWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = MASTER_WINDOW;
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode )
		ShowWindow( fin->hwnd, SW_SHOW );
	SbGUIMaster.masterWnd = fin;
	return fin;
}

sbWnd *ViewcmdMasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ViewcmdMasterWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = VIEWCMD_MASTER_WINDOW;
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
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
	fin->hwnd = StringWindowHWNDCreator( parent, name, ( dimType & 0x3C ), &selfSize );
	fin->specific = StringWindowInfoCreator( str, fontSize, clickable );
	fin->type = STRING_WINDOW;
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode ) {
		ShowWindow( fin->hwnd, SW_SHOW );
		UpdateWindow( fin->hwnd );
	}
	return fin;
}

sbWnd *DldcmdMasterWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = DldcmdMasterWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = DLDCMD_MASTER_WINDOW;
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
	if ( SbGUIMaster.createMode ) {
		ShowWindow( fin->hwnd, SW_SHOW );
		UpdateWindow( fin->hwnd );
	}
	return fin;
}

sbWnd *ProgressBarWindowCreator( HWND parent, const wString name, uint8_t dimType, sbWnd_Dims *dims, uint16_t total ) {
	sbWnd *fin = CommonInfoCreator( parent, name, dimType, dims );
	fin->hwnd = ProgressBarWindowHWNDCreator( parent, name, dimType, dims );
	fin->type = PROGRESS_BAR_WINDOW;
	fin->specific = ProgressBarWindowInfoCreator( total );
	Maps.Insert( SbGUIMaster.WindowMap, fin->hwnd, fin );
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
	int h = r.bottom - r.top;
	fin->currentFont->lfHeight = fin->fontSize;
	
	fin->currentLine = 0;
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

void destroy_SbBasicWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbTextbox( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteTextboxInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbBasicTextWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteBasicTextWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbClickableWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteClickableWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbRestrictedImageWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteRestrictedImageWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbViewcmdMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbStringWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteStringWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbProgressBarWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
	sbWnd_DeleteProgressBarWindowInfo( wnd->specific );
	free( wnd->dims );
	free( wnd );
}

void destroy_SbDldcmdMasterWindow( sbWnd *wnd ) {
	if ( wnd == NULL )
		return;
	Maps.Delete( SbGUIMaster.WindowMap, GetHWND( wnd ) );
	Lists.Delete( wnd->referenceList );
	DestroyWindow( GetHWND( wnd ) );
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

void sbWnd_ProcessQueue( HWND hwnd ) {
	MSG msg = { 0 };
	while ( PeekMessageW( &msg, hwnd, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}
}