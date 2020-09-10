#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gdiplus/gdiplus.h>
#include <windows.h>

#include "sbwnd/sbwnd_init.h"
#include "sb2/sb_ds.h"
#include "sbwnd/sbwnd_create.h"
#include "sbwnd/sbwnd_general.h"
#include "res/sbwnd_res.h"
#include "sbwnd/sbwnd_proc.h"

SbGUIMasterStruct SbGUIMaster;

void initGUI( void ) {
	initDS( );
	initWindowFunctions( );
	SetUpSbGUIMaster( );
	RegisterClasses( );
	CreateTestWindow( );
}

void initWindowFunctions( void ) {
	SBBasicWindows.create = BasicWindowCreator;
	SBBasicWindows.destroy = destroy_SbBasicWindow;
	SBTextboxes.create = TextboxCreator;
	SBTextboxes.destroy = destroy_SbTextbox;
	SBTextboxes.getString = getString_SbTextbox;
	SBTextboxes.sendString = sendString_SbTextbox;
	SBTextboxes.setEnterAction = setEnterAction_SbTextbox;
	SBTextboxes.getEnterAction = getEnterAction_SbTextbox;
	SBBasicTextWindows.create = BasicTextWindowCreator;
	SBBasicTextWindows.destroy = destroy_SbBasicTextWindow;
	SBBasicTextWindows.draw = draw_SbBasicTextWindow;
	SBBasicTextWindows.clear = clear_SbBasicTextWindow;
	SBClickableWindows.create = ClickableWindowCreator;
	SBClickableWindows.destroy = destroy_SbClickableWindow;
	SBRestrictedImageWindows.create = RestrictedImageWindowCreator;
	SBRestrictedImageWindows.destroy = destroy_SbRestrictedImageWindow;
	SBRestrictedImageWindows.updateImage = updateImage_SbRestrictedImageWindow;
	SBMasterWindows.create = MasterWindowCreator;
	SBMasterWindows.destroy = destroy_SbMasterWindow;
	SBViewcmdMasterWindows.create = ViewcmdMasterWindowCreator;
	SBViewcmdMasterWindows.destroy = destroy_SbViewcmdMasterWindow;
	SBStringWindows.create = StringWindowCreator;
	SBStringWindows.destroy = destroy_SbStringWindow;
	SBStringWindows.changeString = changeString_SbStringWindow;
	SBProgressBarWindows.create = ProgressBarWindowCreator;
	SBProgressBarWindows.destroy = destroy_SbProgressBarWindow;
	SBProgressBarWindows.advance = advance_SbProgressBarWindow;
	SBDldcmdMasterWindows.create = DldcmdMasterWindowCreator;
	SBDldcmdMasterWindows.destroy = destroy_SbDldcmdMasterWindow;
	SBWindows.setIn = SetFunctionIn_AllTypes;
	SBWindows.setOut = SetFunctionOut_AllTypes;
	SBWindows.appendReference = appendReference_AllTypes;
	SBWindows.getReference = getReference_AllTypes;
	SBWindows.getStringWidth = getStringWidth_AllTypes;
	SBWindows.in = CallFunctionIn_AllTypes;
	SBWindows.out = CallFunctionOut_AllTypes;
	SBWindows.changeDims = changeDims_AllTypes;
	SBWindows.toSurface = toSurface_AllTypes;
	SBWindows.focus = focus_AllTypes;
	SBWindows.getDims = getDims_AllTypes;
	SBWindows.getParent = getParent_AllTypes;
	SBWindows.getID = getID_AllTypes;
	SBWindows.getType = getType_AllTypes;
	SBWindows.setCreateMode = setCreateMode_AllTypes;
	SBWindows.show = show_AllTypes;
	SBWindows.hide = hide_AllTypes;
}

void SetUpSbGUIMaster( void ) {
	GdiplusStartupInput in = {
		.GdiplusVersion = 1
	};
	GdiplusStartupOutput out;
	ULONG_PTR tok;
	GdiplusStartup( &tok, &in, &out );
	SbGUIMaster.gdipToken = tok;
	SbGUIMaster.large_id = 0;
	SbGUIMaster.createMode = SBWND_CREATEMODE_SHOW;
	SbGUIMaster.WindowMap = Maps.New( );
	SbGUIMaster.WindowClassNameArray = newarr( wString, NUM_CLASSES );
	LOGFONTW *consolasFont = newptr( LOGFONTW );
	wcscpy( consolasFont->lfFaceName, L"Consolas" );
	consolasFont->lfQuality = CLEARTYPE_QUALITY;
	consolasFont->lfCharSet = DEFAULT_CHARSET;
	SbGUIMaster.currentFont = consolasFont;
	SbGUIMaster.sbAccelTable = LoadAccelerators( GetModuleHandleW( NULL ), MAKEINTRESOURCE( TBL_ONE ) );
	SbGUIMaster.TopLevelWindows = Lists.New( );
	SbGUIMaster.masterWnd = NULL;
}

void RegisterClasses( void ) {
	HINSTANCE inst = GetModuleHandleW( NULL );
	WNDCLASSEXW
		txtbox = { 0 },
		basic = { 0 },
		text = { 0 },
		clickable = { 0 },
		restrictedImage = { 0 },
		master = { 0 },
		viewmaster = { 0 },
		string = { 0 },
		progbar = { 0 },
		dldmaster = { 0 };

	SbGUIMaster.WindowClassNameArray[ BASIC_WINDOW ] = L"basic";
	SbGUIMaster.WindowClassNameArray[ TEXTBOX_WINDOW ] = L"textbox";
	SbGUIMaster.WindowClassNameArray[ TEXT_WINDOW ] = L"text";
	SbGUIMaster.WindowClassNameArray[ CLICKABLE_WINDOW ] = L"clickable";
	SbGUIMaster.WindowClassNameArray[ RESTRICTED_IMAGE_WINDOW ] = L"restrictedImage";
	SbGUIMaster.WindowClassNameArray[ MASTER_WINDOW ] = L"master";
	SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ] = L"viewmaster";
	SbGUIMaster.WindowClassNameArray[ STRING_WINDOW ] = L"string";
	SbGUIMaster.WindowClassNameArray[ PROGRESS_BAR_WINDOW ] = L"progbar";
	SbGUIMaster.WindowClassNameArray[ DLDCMD_MASTER_WINDOW ] = L"dldmaster";

	HBRUSH white = CreateSolidBrush( RGB( 0xFF, 0xFF, 0xFF ) );

	txtbox.cbSize = sizeof( WNDCLASSEXW );
	txtbox.lpfnWndProc = TextboxProc;
	txtbox.hInstance = inst;
	txtbox.lpszClassName = SbGUIMaster.WindowClassNameArray[ TEXTBOX_WINDOW ];
	txtbox.hCursor = LoadCursor( NULL, IDC_IBEAM );

	basic.cbSize = sizeof( WNDCLASSEXW );
	basic.lpfnWndProc = BasicWndProc;
	basic.hInstance = inst;
	basic.hCursor = LoadCursor( NULL, IDC_ARROW );
	basic.hbrBackground = white;
	basic.lpszClassName = SbGUIMaster.WindowClassNameArray[ BASIC_WINDOW ];

	text.cbSize = sizeof( WNDCLASSEXW );
	text.lpfnWndProc = TextWndProc;
	text.hInstance = inst;
	text.lpszClassName = SbGUIMaster.WindowClassNameArray[ TEXT_WINDOW ];
	text.hCursor = LoadCursor( NULL, IDC_ARROW );
	text.hbrBackground = white;

	clickable.cbSize = sizeof( WNDCLASSEXW );
	clickable.lpfnWndProc = ClickableProc;
	clickable.hInstance = inst;
	clickable.lpszClassName = SbGUIMaster.WindowClassNameArray[ CLICKABLE_WINDOW ];
	clickable.hCursor = LoadCursor( NULL, IDC_HAND);

	restrictedImage.cbSize = sizeof( WNDCLASSEXW );
	restrictedImage.lpfnWndProc = RestrictedImageProc;
	restrictedImage.hInstance = inst;
	restrictedImage.lpszClassName = SbGUIMaster.WindowClassNameArray[ RESTRICTED_IMAGE_WINDOW ];
	restrictedImage.hCursor = LoadCursor( NULL, IDC_ARROW );
	restrictedImage.hbrBackground = white;

	master.cbSize = sizeof( WNDCLASSEXW );
	master.lpfnWndProc = MasterProc;
	master.hInstance = inst;
	master.lpszClassName = SbGUIMaster.WindowClassNameArray[ MASTER_WINDOW ];
	master.hbrBackground = white;
	master.lpszMenuName = MAKEINTRESOURCEW( MENU_ONE );
	master.hIcon = LoadImageW(
		inst,
		MAKEINTRESOURCEW( LGSB_ICO ),
		IMAGE_ICON,
		0, 0, 0
	);
	master.hIconSm = LoadImageW(
		inst,
		MAKEINTRESOURCEW( SMSB_ICO ),
		IMAGE_ICON,
		0, 0, 0
	);
	master.lpszMenuName = MAKEINTRESOURCEW( MENU_ONE );
	master.hCursor = LoadCursor( NULL, IDC_ARROW );

	viewmaster.cbSize = sizeof( WNDCLASSEXW );
	viewmaster.lpfnWndProc = ViewcmdMasterProc;
	viewmaster.hInstance = inst;
	viewmaster.lpszClassName = SbGUIMaster.WindowClassNameArray[ VIEWCMD_MASTER_WINDOW ];
	viewmaster.hbrBackground = white;

	string.cbSize = sizeof( WNDCLASSEXW );
	string.lpfnWndProc = StringProc;
	string.hInstance = inst;
	string.lpszClassName = SbGUIMaster.WindowClassNameArray[ STRING_WINDOW ];
	string.hbrBackground = white;

	progbar.cbSize = sizeof( WNDCLASSEXW );
	progbar.lpfnWndProc = ProgressBarProc;
	progbar.hInstance = inst;
	progbar.lpszClassName = SbGUIMaster.WindowClassNameArray[ PROGRESS_BAR_WINDOW ];
	progbar.hbrBackground = white;

	dldmaster.cbSize = sizeof( WNDCLASSEXW );
	dldmaster.lpfnWndProc = DldcmdMasterProc;
	dldmaster.hInstance = inst;
	dldmaster.lpszClassName = SbGUIMaster.WindowClassNameArray[ DLDCMD_MASTER_WINDOW ];
	dldmaster.hbrBackground = white;

	RegisterClassExW( &basic );
	RegisterClassExW( &txtbox );
	RegisterClassExW( &text );
	RegisterClassExW( &clickable );
	RegisterClassExW( &restrictedImage );
	RegisterClassExW( &master );
	RegisterClassExW( &viewmaster );
	RegisterClassExW( &string );
	RegisterClassExW( &progbar );
	RegisterClassExW( &dldmaster );
}

void CreateTestWindow( void ) {
	sbWnd_Dims d = { 0 };
	SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
	SbGUIMaster.testWnd = SBBasicWindows.create( NULL, L"testwindow", 0, &d );
	SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );
}

void shutdownSB( void ) {
	HINSTANCE inst = GetModuleHandleW( NULL );
	for ( int i = 0; i < NUM_CLASSES; ++i )
		UnregisterClassW( SbGUIMaster.WindowClassNameArray[i], inst );

	DestroyAcceleratorTable( SbGUIMaster.sbAccelTable );
	free( SbGUIMaster.WindowClassNameArray );
	free( SbGUIMaster.currentFont );
	SBBasicWindows.destroy( SbGUIMaster.testWnd );
	Maps.Destroy( SbGUIMaster.WindowMap );
	Lists.Delete( SbGUIMaster.TopLevelWindows );
	GdiplusShutdown( SbGUIMaster.gdipToken );
	return;
}