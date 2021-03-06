#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <setjmp.h>
#include <wchar.h>

#include "sb2/sb_eng.h"
#include "sb2/sb_file.h"
#include "sb2/sb_net.h"
#include "sb2/sb_proc.h"
#include "sb2/sb_init.h"
#include "sb2/sb_rand.h"
#include "sbwnd/sbgui.h"

CommandFunc cmdFuncs[7];
SbCommandFnStruct SbCommand;
SbEngineFnStruct SbEngine;

// #define PRINT_ERROR

void initEngine( void ) {
	Commands.Number.CopyDownloads = CopyDownloads_SbCommand_Number;
	Commands.Number.Begin = Begin_SbCommand_Number;
	Commands.Number.CreateWindows = CreateWindows_SbCommand_Number;
	Commands.Number.End = End_SbCommand_Number;

	EngineOps.NumCmd.getImagePath = getImagePath_SbEngine_NumCmd;
	EngineOps.NumCmd.updateWindows = updateWindows_SbEngine_NumCmd;
	EngineOps.NumCmd.download = download_SbEngine_NumCmd;
	EngineOps.NumCmd.showWindows = showWindows_SbEngine_NumCmd;

	TDownloads.init = init_SbEngine_TDownload;
	TDownloads.advance = advance_SbEngine_TDownload;
	TDownloads.finish = finish_SbEngine_TDownload;
	TDownloads.create = create_SbEngine_TDownload;


	Commands.Download.CheckConfig = CheckConfig_SbCommand_Download;
	Commands.Download.StorePath = StorePath_SbCommand_Download;
	Commands.Download.Close = Close_SbCommand_Download;

	Commands.Download.Ses.Begin = Begin_SbCommand_Download_Ses;
	Commands.Download.Ses.CreateDynamicWindows = CreateDynamicWindows_SbCommand_Download_Ses;
	Commands.Download.Ses.CreateStaticWindows = CreateStaticWindows_SbCommand_Download_Ses;
	Commands.Download.Ses.End = End_SbCommand_Download_Ses;
	Commands.Download.Ses.InitCoverFolders = InitCoverFolders_SbCommand_Download_Ses;
	Commands.Download.Ses.StartPermDownloads = StartPermDownloads_SbCommand_Download_Ses;

	EngineOps.DldCmd.getPermDldPath = getPermDldPath_SbEngine_DldCmd;
	EngineOps.DldCmd.remDupNumbers = remDupNumbers_SbEngine_DldCmd;
	EngineOps.DldCmd.showWindows = showWindows_SbEngine_DldCmd;

	PDownloads.create = create_SbEngine_PDownload;
	PDownloads.init = init_SbEngine_PDownload;
	PDownloads.advance = advance_SbEngine_PDownload;
	PDownloads.finish = finish_SbEngine_PDownload;

	Commands.Chconf.Confirm = Confirm_SbCmd_Chconf;
}

cmdType sbCmd_GetType( wchar_t *str ) {
	wchar_t cmd[16] = { 0 };
	swscanf( str, L"%15s", cmd );
	if ( !wcscmp( cmd, L"number" ) )
		return ( cmdType )NUMBER;
	else if ( !wcscmp( cmd, L"search" ) )
		return ( cmdType )SEARCH;
	else if ( !wcscmp( cmd, L"preset" ) )
		return ( cmdType )PRESET;
	else if ( !wcscmp( cmd, L"download" ) )
		return ( cmdType )DOWNLOAD;
	else if ( !wcscmp( cmd, L"info" ) )
		return ( cmdType )INFO;
	else if ( !wcscmp( cmd, L"exit" ) )
		return ( cmdType )EXIT;
	else if ( !wcscmp( cmd, L"chconf" ) )
		return ( cmdType )CHCONF;
	return ( cmdType )UNKNOWN;
}

void sbEntry( sbWnd *wnd, void *text ) {
	cmdType type = sbCmd_GetType( text );

	// Reference 0 is viewer, reference 1 is logger.
	sbWnd *viewer = SBWindows.getReference( wnd, 0 );
	sbWnd *logger = SBWindows.getReference( wnd, 1 );

	if ( type == ( cmdType )UNKNOWN ) {
		SBBasicTextWindows.draw( logger, L"Unknown command" );
		return;
	}

	if ( SaucebotMaster.cmdOn && type != ( cmdType )EXIT ) {
		SBBasicTextWindows.draw( logger, L"Cannot execute command" );
		return;
	}

	if ( viewer == NULL || logger == NULL )
		return;

	cmdFuncs[ type ]( viewer, logger, text );
	return;
}



void sbCmd_Number( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	SBBasicTextWindows.clear( viewer );

	wchar_t *num = wcstok( text, L" " );
	sbList *numList = Lists.New( );
	SBBasicTextWindows.draw( logger, L"Opening numbers:" );
	while ( ( num = wcstok( NULL, L" " ) ) != NULL ) {

		wchar_t fin[16] = { 0 };
		if ( !wcscmp( num, L"rand" ) ) {
			uint32_t n = ( uint32_t )( SbRandoms.rand( ) * SaucebotMaster.nMax + 1 );
			swprintf( fin, 16, L"%u", n );
			num = fin;
		}

		Lists.Add( numList, wcsdup( num ) );
		SBBasicTextWindows.draw( logger, wcsdup( num ) );
	}
	
	if ( Lists.Size( numList ) == 0 )
		return;

	SaucebotMaster.cmdOn = 1;
	TDownloads.create( numList );
	sbList *pgsPerNum =  Commands.Number.CopyDownloads( numList );

	// Set up windows and bind functions to said windows.
	sbWnd_Layout lout = { 0 };

	// Master window
	lout.parent = viewer;
	lout.type = SB_DIMTYPE_IIFF_TL_SRNN;
	lout.dims.floatDims[2] = 1.0f;
	lout.dims.floatDims[3] = 1.0f;

	sbWnd *viewMaster = SBViewcmdMasterWindows.create( L"Viewer master window", &lout );

	// Number switcher left arrow
	lout.parent = viewMaster;
	lout.type = SB_DIMTYPE_FIII_TL_SRNN;
	lout.dims.floatDims[0] = SB_CENTER_DIM( 0.4f );

	sbWnd *lArrow = SBStringWindows.create( L"left arrow 1", &lout, L"<", 15, 1 );

	// Number switcher right arrow
	lout.dims.floatDims[0] = SB_CENTER_DIM( 0.6f );

	sbWnd *rArrow = SBStringWindows.create( L"right arrow 1", &lout, L">", 15, 1 );

	// Close button
	lout.dims.intDims[0] = 0;
	lout.type = SB_DIMTYPE_IIII_TR_SRNN;

	sbWnd *closeButton = SBStringWindows.create( L"close button", &lout, L"Close", 15, 1 );

	// Fullscreen button
	lout.type = SB_DIMTYPE_IIII_TL_SRNN;

	sbWnd *fscreenButton = SBStringWindows.create( L"fullscreen button", &lout, L"Fullscreen", 15, 1 );

	// Loading symbol
	lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
	lout.dims.floatDims[1] = SB_CENTER_DIM( 0.5f );
	lout.type = SB_DIMTYPE_FFII_TL_SRNN;

	sbWnd *load = SBStringWindows.create( L"load", &lout, L"Loading...", 15, 0 );

	SBWindows.setSignalFn( viewMaster, numCmdProc );
	SBWindows.setSignalFn( lArrow, SbNumCmdEvents.prevNum );
	SBWindows.setSignalFn( rArrow, SbNumCmdEvents.nextNum );
	SBWindows.setSignalFn( closeButton, SbNumCmdEvents.close );
	SBWindows.setSignalFn( fscreenButton, SbNumCmdEvents.fullscreen );

	SBWindows.appendReference( lArrow, viewMaster );
	SBWindows.appendReference( rArrow, viewMaster );
	SBWindows.appendReference( closeButton, viewMaster );
	SBWindows.appendReference( fscreenButton, viewMaster );

	sbCmd_Number_Session *ses = Commands.Number.Begin( viewMaster, numList, pgsPerNum );
	Lists.Delete( pgsPerNum );
	SBWindows.destroy( load );
	SBWindows.appendReference( viewMaster, ses );

	Commands.Number.CreateWindows( ses );
	if ( !SaucebotMaster.offlineMode )
		EngineOps.NumCmd.download( ses );
	EngineOps.NumCmd.updateWindows( ses );
	return;
}

void create_SbEngine_TDownload( sbList *nlist ) {
	wchar_t path[ MAX_PATH ] = { 0 };
	swprintf( path, MAX_PATH, L"TempDownloads\\" );
	if ( !Lists.Size( nlist ) ) return;
	uint16_t i = 0;
	do {
		CreateDirectoryW( path, NULL );
		ZeroMemory( path, MAX_PATH );
		swprintf( path, MAX_PATH, L"TempDownloads\\%ls\\", Lists.Get( nlist, i ) );
	} while ( ++i <= Lists.Size( nlist ) );
}

sbList *CopyDownloads_SbCommand_Number( sbList *numList ) {
	sbList *fin = Lists.New( );
	wchar_t *path = EngineOps.DldCmd.getPermDldPath( );

	for ( uint16_t i = 0; i < Lists.Size( numList ); ++i ) {
		if ( path == NULL ) {
			Lists.Add( fin, ( void * )0 );
			continue;
		}

		wchar_t
			dst[ MAX_PATH ] = { 0 },
			*n = Lists.Get( numList, i ),
			src[ MAX_PATH ] = { 0 };
		swprintf( dst, MAX_PATH, L"TempDownloads\\%s", n );
		swprintf( src, MAX_PATH, L"%s\\%s", path, n );

		uint64_t c = Files.copyDir( dst, src );
		c -= c > 0 ? 1 : 0;
		Lists.Add( fin, ( void * )c );
	}
	return fin;
}

sbCmd_Number_Session *Begin_SbCommand_Number( sbWnd *master, sbList *nlist, sbList *pgsDld ) {
	sbCmd_Number_Session *ses = newptr( sbCmd_Number_Session );
	uint16_t nCt = Lists.Size( nlist );

	ses->windows = newarr( sbList *, nCt );
	for ( uint16_t i = 0; i < nCt; ++i )
		ses->windows[i] = Lists.New( );

	ses->sessionParent = master;
	ses->numList = nlist;
	ses->currentPgs = Lists.New( );
	for ( uint16_t i = 0; i < nCt; ++i )
		Lists.Add( ses->currentPgs, ( void * )1 );

	ses->tDownloads = newarr( sb_TempDownload, nCt );
	TDownloads.init( ses->tDownloads, nlist );
	for ( uint16_t i = 0; i < nCt; ++i ) {
		uint16_t a = ( uint64_t )Lists.Get( pgsDld, i );
		ses->tDownloads[i].nPgs = ( uint64_t )Lists.Get( pgsDld, i );
	}

	sbWnd_Layout *fscreen = newptr( sbWnd_Layout );
	ses->nfscreen = newptr( sbWnd_Layout );
	
	fscreen->parent = SBWindows.getParent(
		SBWindows.getParent( master )
	);
	fscreen->dims.floatDims[2] = 1.0f;
	fscreen->dims.floatDims[3] = 1.0f;
	fscreen->type = SB_DIMTYPE_IIFF_TL_SRNN;
	ses->fscreen = fscreen;

	SBWindows.getLayout( SBWindows.getParent( master ), ses->nfscreen );

	for ( int i = 0; i < nCt; ++i )
		if ( ses->tDownloads[i].nPgs == 0 && !SaucebotMaster.offlineMode )
			TDownloads.advance( ( ses->tDownloads + i ) );

	return ses;
}

void init_SbEngine_TDownload( sb_TempDownload *dArr, sbList *nlist ) {
	for ( uint16_t i = 0; i < Lists.Size( nlist ); ++i ) {
		// Check for offline mode, if so input page count from info file (if there is one) into tpgs.
		if ( SaucebotMaster.offlineMode ) {
			wchar_t
				*n = Lists.Get( nlist, i ),
				path[ MAX_PATH ] = { 0 };
			swprintf( path, MAX_PATH, L"TempDownloads\\%s\\info", n );

			{
				WIN32_FIND_DATAW fdt = { 0 };
				HANDLE ses = FindFirstFileW( path, &fdt );
				if ( GetLastError( ) == ERROR_FILE_NOT_FOUND )
					continue;
				FindClose( ses );
			}

			{
				FILE *f = _wfopen( path, L"r" );
				HTML_ops.seekString( f, L"pages" );
				wchar_t *pg = HTML_ops.readBetween( f, L'\"', L'\"' );
				swscanf( pg, L"%u", &dArr[i].tPgs );
				fclose( f );
				free( pg );
			}

			continue;
		}

		// Get number
		wchar_t *n = Lists.Get( nlist, i );
		wcscpy( dArr[i].number, n );

		// Get n/1 into temp.txt
		{
			wchar_t url[16] = { 0 };
			swprintf( url, 16, L"%ls/1", n );
			SbNet.numget( url, L"temp.txt" );
		}
		FILE *f = _wfopen( L"temp.txt", L"r" );

		// Page count
		{
			HTML_ops.seekString( f, L"num-pages" );
			wchar_t *pg = HTML_ops.readBetween( f, L'>', L'<' );
			swscanf( pg, L"%d", &dArr[i].tPgs );
			free( pg );
		}

		// Gallery number
		{
			HTML_ops.seekString( f, L"img src" );
			wchar_t *galUrl = HTML_ops.readBetween( f, L'\"', L'\"' );
			swscanf( galUrl, L"https://i.nhentai.net/galleries/%[^/]", dArr[i].gallery );
			free( galUrl );
		}
		fclose( f );
	}
}

void advance_SbEngine_TDownload( sb_TempDownload *dld ) {
	/* 
	 * This is a new implementation of advanceDownload.
	 * Supporting more file types is trivial, as only new variables are needed along with new requests.
	 * Adding another file type would only introduce a small speed reduction.
	 */

	// Don't download if it's already all downloaded.
	if ( dld->nPgs >= dld->tPgs )
		return;
	dld->nPgs++;

	// Attempt both a jpg and a png, ASCII string needed.
	char
		jpg[256] = { 0 },
		png[256] = { 0 };
	sprintf( jpg, "https://i.nhentai.net/galleries/%ls/%d.jpg", dld->gallery, dld->nPgs );
	sprintf( png, "https://i.nhentai.net/galleries/%ls/%d.png", dld->gallery, dld->nPgs );

	#ifdef PRINT_ERROR
	printf( "URL: %s\n", jpg );
	#endif

	// Paths for both images. One of the two images is garunteed to fail.
	wchar_t
		jpgPath[ MAX_PATH ] = { 0 },
		pngPath[ MAX_PATH ] = { 0 };
	swprintf( jpgPath, MAX_PATH, L"TempDownloads\\%ls\\%d.jpg", dld->number, dld->nPgs );
	swprintf( pngPath, MAX_PATH, L"TempDownloads\\%ls\\%d.png", dld->number, dld->nPgs );

	// Attempt both. One returns a 404 and the file is never created, the other succeeds.
	SbNet.imgget( jpg, jpgPath );
	SbNet.imgget( png, pngPath );
}

wchar_t *getImagePath_SbEngine_NumCmd( wchar_t *num, uint16_t pg ) {
	wchar_t *fin = newwstr( MAX_PATH );
	swprintf( fin, MAX_PATH, L"TempDownloads\\%ls\\%d.*", num, pg );

	WIN32_FIND_DATAW fd = { 0 };
	HANDLE h = FindFirstFileW( fin, &fd );
	
	#ifdef PRINT_ERROR
	printf( "Filename: %ls\n", fd.cFileName );
	#endif
	ZeroMemory( fin, MAX_PATH );
	swprintf( fin, MAX_PATH, L"TempDownloads\\%ls\\%ls", num, fd.cFileName );

	FindClose( h );

	return fin;
}

void CreateWindows_SbCommand_Number( sbCmd_Number_Session *ses ) {
	SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		sbWnd_Layout lout = { 0 };

		// Number window
		lout.parent = ses->sessionParent;
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.type = SB_DIMTYPE_FIII_TL_SRNN;

		sbWnd *num = SBStringWindows.create( L"name", &lout, Lists.Get( ses->numList, i ), 15, 0 );
		
		// Center image window
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.floatDims[1] = 0.05f;
		lout.dims.floatDims[2] = 0.9f;
		lout.dims.floatDims[3] = 0.9f;
		lout.type = SB_DIMTYPE_FFFF_TL_SRNN;

		sbWnd *imgWnd = SBRestrictedImageWindows.create( L"image", &lout, NULL );

		// Left Arrow
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.4f );
		lout.dims.intDims[1] = 0;
		lout.dims.intDims[2] = 0;
		lout.dims.intDims[3] = 0;
		lout.type = SB_DIMTYPE_FIII_BL_SRNN;

		sbWnd *lArrow = SBStringWindows.create( L"name", &lout, L"<", 15, 1 );

		// Right Arrow
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.6f );

		sbWnd *rArrow = SBStringWindows.create( L"name", &lout, L">", 15, 1 );

		// Page counter window
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );

		sbWnd *pages = SBStringWindows.create( L"page counter", &lout, L"0 of 0", 15, 0 );

		// Status bar
		lout.dims.intDims[0] = 0;
		lout.type = SB_DIMTYPE_IIII_BR_SRNN;

		sbWnd *status = SBStringWindows.create( L"status bar", &lout, L"Downloaded: 0/0", 15, 0	);

		SBWindows.setSignalFn( lArrow, SbNumCmdEvents.prevPage );
		SBWindows.setSignalFn( rArrow, SbNumCmdEvents.nextPage );

		SBWindows.appendReference( lArrow, ses->sessionParent );
		SBWindows.appendReference( rArrow, ses->sessionParent );

		Lists.Add( ses->windows[i], num );
		Lists.Add( ses->windows[i], imgWnd );
		Lists.Add( ses->windows[i], lArrow );
		Lists.Add( ses->windows[i], rArrow );
		Lists.Add( ses->windows[i], pages );
		Lists.Add( ses->windows[i], status );

	}
	SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );
}

void updateWindows_SbEngine_NumCmd( sbCmd_Number_Session *ses ) {
	sbList *windowList = ses->windows[ ses->idx ];
	for ( uint16_t i = 0; i < Lists.Size( windowList ); ++i ) {
		sbWnd *wnd = Lists.Get( windowList, i );
		SBWindows.show( wnd, 1 );

		wchar_t name[ 256 ] = { 0 };
		GetWindowTextW( GetHWND( wnd ), name, 256 );

		if ( !wcscmp( L"image", name ) ) {
			wchar_t *num = Lists.Get( ses->numList, ses->idx );
			uint16_t pg = ( uint64_t )Lists.Get( ses->currentPgs, ses->idx );
			wchar_t *imgPath = EngineOps.NumCmd.getImagePath( num, pg );
			SBRestrictedImageWindows.updateImage( wnd, imgPath );
			free( imgPath );
		}

		if ( !wcscmp( L"page counter", name ) ) {
			uint16_t pg = ( uint64_t )Lists.Get( ses->currentPgs, ses->idx );
			sb_TempDownload dld = ses->tDownloads[ ses->idx ];
			wchar_t newCounter[ 64 ] = { 0 };
			swprintf( newCounter, 64, L"%d of %d", pg, dld.tPgs );
			SBStringWindows.changeString( wnd, newCounter );
		}

		if ( !wcscmp( L"status bar", name ) ) {
			sb_TempDownload dld = ses->tDownloads[ ses->idx ];
			wchar_t msg[ 64 ] = { 0 };
			swprintf( msg, 64, L"Downloaded: %d/%d", dld.nPgs, dld.tPgs );
			SBStringWindows.changeString( wnd, msg );
		}
	}
}

void End_SbCommand_Number( sbCmd_Number_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		sbList *windowList = ses->windows[i];
		for ( uint16_t j = 0; j < Lists.Size( windowList ); ++j ) {
			sbWnd *wnd = Lists.Get( windowList, j );
			SBWindows.destroy( wnd );
		}
		Lists.Delete( windowList );
	}
	
	free( ses->windows );
	Lists.Delete( ses->currentPgs );
	Lists.Delete( ses->numList );
	free( ses->fscreen );
	free( ses->nfscreen );
	SBWindows.destroy( ses->sessionParent );

	// Signal dowloader threads to end
	for ( uint16_t i = 0; i < Lists.Size( SaucebotMaster.threadHandleList ); ++i )
		ses->tDownloads[i].signal = 1;

	for ( uint16_t i = 0; i < Lists.Size( SaucebotMaster.threadHandleList ); ++i ) {
		_cwait( NULL, ( intptr_t )Lists.Get( SaucebotMaster.threadHandleList, i ), 0 );
		Lists.Remove( SaucebotMaster.threadHandleList, i-- );
	}

	free( ses->tDownloads );
	Files.recursiveDelete( L"TempDownloads" );
	SaucebotMaster.cmdOn = 0;
}

struct downloadThreadInfo {
	sbWnd *wnd;
	sb_TempDownload *dld;
};

void download_SbEngine_NumCmd( sbCmd_Number_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		sb_TempDownload *dld = ( ses->tDownloads + i );
		struct downloadThreadInfo *dInf = newptr( struct downloadThreadInfo );

		dInf->dld = dld;
		sbList *windowList = ses->windows[i];
		dInf->wnd = Lists.Get( windowList, 5 ); // "x of y downloaded" window

		uintptr_t th = _beginthread( TDownloads.finish, 0, dInf );
		Lists.Add( SaucebotMaster.threadHandleList, ( void * )th );
	}
}

void finish_SbEngine_TDownload( void *data ) { // Used with _beginthread ONLY!
	struct downloadThreadInfo *dldInfo = data;
	
	sb_TempDownload *dld = dldInfo->dld;
	sbWnd *wnd = dldInfo->wnd;

	for ( uint16_t i = dld->nPgs; i < dld->tPgs; ++i ) {
		TDownloads.advance( dld );
		wchar_t status[32] = { 0 };
		swprintf( status, 32, L"Downloaded: %d/%d", dld->nPgs, dld->tPgs );
		SBStringWindows.changeString( wnd, status );

		

		#ifdef PRINT_ERROR
		printf( "Signal: %d\n", dld->signal );
		#endif

		if ( dld->signal )
			_endthread( );
	}

	free( data );
}

void showWindows_SbEngine_NumCmd( sbCmd_Number_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i )
		for ( uint16_t j = 0; j < Lists.Size( ses->windows[i] ); ++j ) {
			if ( i != ses->idx )
				SBWindows.hide( Lists.Get( ses->windows[i], j ), 1 );
			else
				SBWindows.show( Lists.Get( ses->windows[i], j ), 1 );
		}
}

void sbCmd_Download( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	// Don't even try if offline mode is on.
	if ( SaucebotMaster.offlineMode ) {
		SBBasicTextWindows.draw( logger, L"Cannot download, offline mode on." );
		return;
	}

	// Begin parsing
	SBBasicTextWindows.clear( viewer );
	wchar_t *num = wcstok( text, L" " );
	sbList *numList = Lists.New( );
	SBBasicTextWindows.draw( logger, L"Downloading numbers:" );
	while ( ( num = wcstok( NULL, L" " ) ) != NULL ) {

		wchar_t fin[16] = { 0 };
		if ( !wcscmp( num, L"rand" ) ) {
			uint32_t n = ( uint32_t )( SbRandoms.rand( ) * SaucebotMaster.nMax + 1 );
			swprintf( fin, 16, L"%u", n );
			num = fin;
		}

		Lists.Add( numList, wcsdup( num ) );
	}

	EngineOps.DldCmd.remDupNumbers( numList );
	if ( !Lists.Size( numList ) )
		return;
	SaucebotMaster.cmdOn = 1;

	for ( uint16_t i = 0; i < Lists.Size( numList ); ++i )
		SBBasicTextWindows.draw( logger, wcsdup( Lists.Get( numList, i ) ) );
	// End parsing

	/*
	 * Config checking--quickly and easily setting up input
	 * and calling another message loop.
	 * 
	 * This is good for quick inputs like these,
	 * but for complex screens with user interaction,
	 * this is not a good approach.
	 * 
	 * Overall, this trades program simplicity
	 * for stack usage and limited capability.
	 */
	if ( Commands.Download.CheckConfig( ) ) {
		jmp_buf *jmp = newptr( jmp_buf );
		int v = setjmp( *jmp );

		sbWnd 
			TBOX *inputBox,
			STRWND *prompt,
			STRWND *invalid,
			STRWND *close;
		
		if ( !v ) {
			// Create windows and call msgloop(), nothing has happened yet
			sbWnd_Layout lout = { 0 };

			lout.parent = viewer;
			lout.type = SB_DIMTYPE_FIII_TL_SRNN;
			lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
			lout.dims.intDims[1] = SB_CENTER_DIM( 150 );
			lout.dims.intDims[2] = 600;
			lout.dims.intDims[3] = 19;

			inputBox = SBTextboxes.create( L"Input box", &lout, 0 );

			lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
			lout.dims.intDims[1] = SB_CENTER_DIM( 130 );

			prompt = SBStringWindows.create( 
				L"path input prompt", &lout,
				L"Enter an absolute path for permanant downloads:", 15, 0
			);

			lout.dims.intDims[1] = SB_CENTER_DIM( 170 );

			SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
			invalid = SBStringWindows.create(
				L"invalidation window", &lout,
				L"Placeholder fail string", 15, 0
			);
			SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );

			lout.dims.intDims[0] = 0;
			lout.dims.intDims[1] = 0;
			lout.type = SB_DIMTYPE_IIII_TR_SRNN;

			close = SBStringWindows.create( L"close", &lout, L"Close", 15, 1 );

			SBWindows.appendReference( close, jmp );
			SBWindows.appendReference( inputBox, jmp ); // ref 0
			SBWindows.appendReference( inputBox, invalid ); // ref 1
			SBWindows.setSignalFn( inputBox, Commands.Download.StorePath );
			SBWindows.setSignalFn( close, Commands.Download.Close );
			SBWindows.focus( inputBox );

			msgloop( ); // longjmp() is called in Commands.Download.StorePath, the stack then unwinds back to the setjmp().

		} else if ( v == 1 ) {
			// Destroy windows, setjmp returned again
			SBWindows.destroy( inputBox );
			SBWindows.destroy( prompt );
			SBWindows.destroy( invalid );
			SBWindows.destroy( close );
		} else {
			// Destroy windows, setjmp returned again and the close button was pressed.
			SBWindows.destroy( inputBox );
			SBWindows.destroy( prompt );
			SBWindows.destroy( invalid );
			SBWindows.destroy( close );
			SaucebotMaster.cmdOn = 0;
			return;
		}

		free( jmp );
	}

	
	/*
	 * Steps for command
	 * Create folders in permanant path ( /path/to/saucebot/Covers/numbers/cover.jpg )
	 * Create stable windows
	 * Download covers
	 * Create changing windows
	 * Set window references and out functions
	 */

	// Loading symbol
	sbCmd_Download_Session *ses;
	sbWnd *loading;
	{
		sbWnd_Layout lout = { 0 };
		lout.parent = viewer;
		lout.type = SB_DIMTYPE_FFII_TL_SRNN;
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.floatDims[1] = SB_CENTER_DIM( 0.5f );

		loading = SBStringWindows.create(
			L"loading", &lout,
			L"Loading...", 15, 0
		);
	}

	{
		ses = Commands.Download.Ses.Begin( viewer, numList );
		Commands.Download.Ses.InitCoverFolders( ses );

		SBWindows.destroy( loading );

		Commands.Download.Ses.CreateStaticWindows( ses );
		Commands.Download.Ses.CreateDynamicWindows( ses );
		Commands.Download.Ses.StartPermDownloads( ses );
	}

	EngineOps.DldCmd.showWindows( ses );
	
	return;
}

int CheckConfig_SbCommand_Download( void ) {
	FILE *f = _wfopen( L"config", L"r" );
	if ( HTML_ops.seekString( f, L"permdldpath" ) )
		return 1;
	return 0;
}

void StorePath_SbCommand_Download( sbWnd *tbox, void *text ) {
	/*
	 * You might think that this looks dangerous, and at first glance, indeed it does.
	 * However, due to the fact the application chooses what functions to bind to the windows,
	 * this is only ever used once.
	 */

	// Reference 0 is jmp_buf, ref 1 is 'invalid' window
	sbWnd *invalid = SBWindows.getReference( tbox, 1 );
	int res = Files.createAbsPath( text );
	switch ( res ) {
		case 1:
			SBStringWindows.changeString(
				invalid,
				L"Drive letter is invalid, enter a different drive letter."
			);
			SBWindows.show( invalid, 1 );
			return;
		case 2: {
			wchar_t msg[ MAX_PATH + 64 ] = { 0 };
			swprintf(
				msg,
				( MAX_PATH + 64 ),
				L"Could not create path %s, enter another path",
				( wchar_t * )text
			);
			SBStringWindows.changeString( invalid, msg );
			SBWindows.show( invalid, 1 );
			return;
		}
	}

	{
		FILE *f = _wfopen( L"config", L"a" );
		fwprintf( f, L"permdldpath=\"%s\"", ( wchar_t * )text );
		fclose( f );
	}

	jmp_buf *jump = ( jmp_buf * )( SBWindows.getReference( tbox, 0 ) );
	longjmp( *jump, 1 );
}

void Close_SbCommand_Download( sbWnd *wnd, void *unused ) {
	jmp_buf *jump = SBWindows.getReference( wnd, 0 );
	longjmp( *jump, 2 );
	return;
}

wchar_t *getPermDldPath_SbEngine_DldCmd( void ) {
	FILE *f = _wfopen( L"config", L"r" );
	int a = HTML_ops.seekString( f, L"permdldpath" );
	if ( !a )
		return HTML_ops.readBetween( f, L'\"', L'\"' );
	
	return NULL;
}

void remDupNumbers_SbEngine_DldCmd( sbList *nlist ) {
	wchar_t
		path[ MAX_PATH ],
		*ppath = EngineOps.DldCmd.getPermDldPath( );

	for ( uint16_t i = 0; i < Lists.Size( nlist ); ++i ) {
		ZeroMemory( path, MAX_PATH );
		swprintf( path, MAX_PATH, L"%s\\%s", ppath, Lists.Get( nlist, i ) );

		WIN32_FIND_DATAW fd = { 0 };
		HANDLE h = FindFirstFileW( path, &fd );

		if ( h != INVALID_HANDLE_VALUE )
			Lists.Remove( nlist, i-- );
		else
			FindClose( h );
	}

	free( ppath );
}

void showWindows_SbEngine_DldCmd( sbCmd_Download_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i )
		for ( uint16_t j = 0; j < Lists.Size( ses->windows[i] ); ++j ) {
			sbWnd *w = Lists.Get( ses->windows[i], j );
			if ( i != ses->idx )
				SBWindows.hide( w, 0 );
			else
				SBWindows.show( w, 0 );
		}
}

void create_SbEngine_PDownload( sbList *nlist ) {
	wchar_t
		path[ MAX_PATH ] = { 0 },
		*ppath = EngineOps.DldCmd.getPermDldPath( );

	if ( !Lists.Size( nlist ) )
		return;

	for ( uint16_t i = 0; i < Lists.Size( nlist ); ++i ) {
		ZeroMemory( path, MAX_PATH );
		wchar_t *n = Lists.Get( nlist, i );
		swprintf( path, MAX_PATH, L"%s\\%s", ppath, n );
		CreateDirectoryW( path, NULL );
		wcscat( path, L"\\info" );
		fclose( _wfopen( path, L"a" ) );
	}
}

void init_SbEngine_PDownload( sb_PermDownload *arr, sbList *nlist ) {
	// Assumes memory already allocated
	wchar_t *ppath = EngineOps.DldCmd.getPermDldPath( );
	for ( uint16_t i = 0; i < Lists.Size( nlist ); ++i ) {
		wchar_t
			infoPath[ MAX_PATH ] = { 0 },
			*n = Lists.Get( nlist, i );

		wcscpy( arr[i].number, n );
		swprintf( arr[i].path, MAX_PATH, L"%s\\%s", ppath, n );
		swprintf( infoPath, MAX_PATH, L"%s\\%s\\info", ppath, n );

		SbNet.numget( n, L"temp.txt" ); // Use main site for title/artist/tags

		FILE *f = _wfopen( L"temp.txt", L"r" );

		// Behold, the power of my parsing engine!

		// Gallery number
		{
			HTML_ops.seekString( f, L"meta itemprop=\"image\" content" );
			wchar_t *galUrl = HTML_ops.readBetween( f, L'\"', L'\"' );
			swscanf( galUrl, L"https://t.nhentai.net/galleries/%[^/]", arr[i].gallery );
			free( galUrl );
		}

		// Title
		{
			HTML_ops.seekString( f, L"meta property=\"og:title\" content" );
			wchar_t *title = HTML_ops.readBetween( f, L'\"', L'\"' );
			wcscpy( arr[i].title, title );
			free( title );
		}

		// Tags
		{
			HTML_ops.seekString( f, L"meta name=\"twitter:description\" content");
			wchar_t *tags = HTML_ops.readBetween( f, L'\"', L'\"' );
			wcscpy( arr[i].tags, tags ); // thank you based nhentai
			free( tags );
		}

		// Artist
		{
			HTML_ops.seekString( f, L"Artists:" );
			HTML_ops.seekString( f, L"<span class=\"tags\">" );
			wchar_t *artist = HTML_ops.readBetween( f, L'\"', L'\"' );
			swscanf( artist, L"/artist/%[^/]", arr[i].author );
			free( artist );
		}

		// Page count
		{
			HTML_ops.seekString( f, L"Pages:" );
			HTML_ops.seekString( f, L"span class=\"name\"" );
			wchar_t *pg = HTML_ops.readBetween( f, L'>', L'<' );
			swscanf( pg, L"%u", &arr[i].tPgs );

			{
				FILE *f = _wfopen( infoPath, L"a" );
				fwprintf( f, L"pages=\"%s\"\n", pg );
				fclose( f );
			}

			free( pg );
		}
		
		fclose( f );
	}
	free( ppath );
}

void advance_SbEngine_PDownload( sb_PermDownload *dld ) {
	/* 
	 * This is a new implementation of advanceDownload.
	 * Supporting more file types is trivial, as only new variables are needed along with new requests.
	 * Adding another file type would only introduce a small speed reduction.
	 */

	wchar_t *ppath = EngineOps.DldCmd.getPermDldPath( );
	// Don't download if it's already all downloaded.
	if ( dld->nPgs >= dld->tPgs )
		return;
	dld->nPgs++;

	// Attempt both a jpg and a png, ASCII string needed.
	char
		jpg[256] = { 0 },
		png[256] = { 0 };
	sprintf( jpg, "https://i.nhentai.net/galleries/%ls/%d.jpg", dld->gallery, dld->nPgs );
	sprintf( png, "https://i.nhentai.net/galleries/%ls/%d.png", dld->gallery, dld->nPgs );

	#ifdef PRINT_ERROR
	printf( "URL: %s\n", jpg );
	#endif

	// Paths for both images. One of the two images is garunteed to fail.
	wchar_t
		jpgPath[ MAX_PATH ] = { 0 },
		pngPath[ MAX_PATH ] = { 0 };
	swprintf( jpgPath, MAX_PATH, L"%s\\%s\\%d.jpg", ppath, dld->number, dld->nPgs );
	swprintf( pngPath, MAX_PATH, L"%s\\%s\\%d.png", ppath, dld->number, dld->nPgs );

	// Attempt both. One returns a 404 and the file is never created, the other succeeds.
	SbNet.imgget( jpg, jpgPath );
	SbNet.imgget( png, pngPath );
	free( ppath );
}


struct pDownloadThreadInfo {
	sbWnd
		*progressBar,
		*progressString;
	sb_PermDownload *dld;
};

void finish_SbEngine_PDownload( void *data ) { // Used with _beginthread() ONLY!
	// Import data
	struct pDownloadThreadInfo *inf = data;
	sbWnd
		PROGBARWND *progressBar = inf->progressBar,
		STRWND *progressString = inf->progressString;
	sb_PermDownload *dld = inf->dld;

	for ( uint16_t i = dld->nPgs; i < dld->tPgs; ++i ) {
		PDownloads.advance( dld );

		if ( dld->signal )
			_endthread( );

		wchar_t newStr[48] = { 0 };
		swprintf( newStr, 48, L"Downloading: %u of %u ~ Number %s", ( i + 1 ), dld->tPgs, dld->number );

		// Remember to sync your painting operations when you use multithreading.
		SBStringWindows.changeString( progressString, wcsdup( newStr ) );
		SBWindows.update( progressString );
		SBProgressBarWindows.advance( progressBar );
		SBWindows.update( progressBar );
		
	}
}

sbCmd_Download_Session *Begin_SbCommand_Download_Ses( sbWnd *viewMaster, sbList *nlist ) {
	sbCmd_Download_Session *ses = newptr( sbCmd_Download_Session );

	ses->nfscreen = newptr( sbWnd_Layout );
	SBWindows.getLayout( viewMaster, ses->nfscreen );

	sbWnd_Layout fscDims = { 0 };
	fscDims.dims.floatDims[2] = 1.0f;
	fscDims.dims.floatDims[3] = 1.0f;
	fscDims.type = SB_DIMTYPE_IIFF_TL_SRNN;
	fscDims.parent = ses->nfscreen->parent;

	ses->fscreen = newptr( sbWnd_Layout );
	memcpy( ses->fscreen, &fscDims, sizeof( sbWnd_Layout ) );

	// Session master
	sbWnd *dldmaster;
	{
		sbWnd_Layout lout = { 0 };
		lout.parent = viewMaster;
		lout.type = SB_DIMTYPE_IIFF_TL_SRNN;
		lout.dims.floatDims[2] = 1.0f;
		lout.dims.floatDims[3] = 1.0f;

		dldmaster = SBDldcmdMasterWindows.create( L"dldmaster", &lout );
	}

	ses->sessionParent = dldmaster;
	ses->numList = nlist;

	uint16_t sz = Lists.Size( nlist );
	ses->windows = newarr( sbList *, sz );
	for ( uint16_t i = 0; i < sz; ++i )
		ses->windows[i] = Lists.New( );

	ses->pDownloads = newarr( sb_PermDownload, sz );
	PDownloads.create( nlist );
	PDownloads.init( ses->pDownloads, nlist );

	return ses;
}

void InitCoverFolders_SbCommand_Download_Ses( sbCmd_Download_Session *ses ) {
	wchar_t path[ MAX_PATH ] = { 0 };
	swprintf( path, MAX_PATH, L"%s", L"Covers" );
	CreateDirectoryW( path, NULL );

	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		wchar_t
			*n = Lists.Get( ses->numList, i ),
			url[384] = { 0 };

		sb_PermDownload *dld = ses->pDownloads + i;
		ZeroMemory( path, MAX_PATH );
		swprintf( path, MAX_PATH, L"Covers\\%s", n );
		CreateDirectoryW( path, NULL );

		ZeroMemory( path, MAX_PATH );
		swprintf( path, MAX_PATH, L"Covers\\%s\\cover.jpg", n );
		swprintf( url, 384, L"https://t.nhentai.net/galleries/%s/cover.jpg", dld->gallery );

		SbNet.imgget_tsite( url, path );
	}
}

void CreateStaticWindows_SbCommand_Download_Ses( sbCmd_Download_Session *ses ) {
	// windows needed
	sbWnd
		*fscreenButton,
		*lArrow,
		*rArrow,
		*closeButton;
	// dimension struct
	sbWnd_Layout lout = { 0 };
	lout.parent = ses->sessionParent;

	// Fullscreen button
	lout.dims.intDims[0] = 0; // Does nothing, just idiomatic
	lout.type = SB_DIMTYPE_IIII_TL_SRNN;

	fscreenButton = SBStringWindows.create(
		L"fscreen-button", &lout,
		L"Fullscreen", 15, 1
	);

	// lArrow
	lout.dims.floatDims[0] = SB_CENTER_DIM( 0.4f );
	lout.type = SB_DIMTYPE_FIII_TL_SRNN;

	lArrow = SBStringWindows.create( L"lArrow", &lout, L"<", 15, 1 );

	// rArrow
	lout.dims.floatDims[0] = SB_CENTER_DIM( 0.6f );

	rArrow = SBStringWindows.create( L"rArrow", &lout, L">", 15, 1 );

	// close button
	lout.dims.intDims[0] = 0;
	lout.type = SB_DIMTYPE_IIII_TR_SRNN;

	closeButton = SBStringWindows.create(
		L"close-button", &lout,
		L"Close", 15, 1
	);

	// Function binding and reference appending
	SBWindows.appendReference( fscreenButton, ses->sessionParent );
	SBWindows.appendReference( lArrow, ses->sessionParent );
	SBWindows.appendReference( rArrow, ses->sessionParent );
	SBWindows.appendReference( closeButton, ses->sessionParent );
	SBWindows.appendReference( ses->sessionParent, ses );

	SBWindows.setSignalFn( fscreenButton, SbDldCmdEvents.fullscreen );
	SBWindows.setSignalFn( lArrow, SbDldCmdEvents.prevNum );
	SBWindows.setSignalFn( rArrow, SbDldCmdEvents.nextNum );
	SBWindows.setSignalFn( closeButton, SbDldCmdEvents.close );
	SBWindows.setSignalFn( ses->sessionParent, dldCmdProc );
}

void CreateDynamicWindows_SbCommand_Download_Ses( sbCmd_Download_Session *ses ) {
	SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		wchar_t *n = Lists.Get( ses->numList, i );
		sb_PermDownload *dld = ses->pDownloads + i;
		sbWnd_Layout lout = { 0 };

		sbWnd
			*numWnd,
			*coverImgWnd,
			*infoBox,
			*progStr,
			*progBar,
			*progMsg;

		lout.parent = ses->sessionParent;
		
		// Number window
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.type = SB_DIMTYPE_FIII_TL_SRNN;
		numWnd = SBStringWindows.create( L"numWnd", &lout, n, 15, 0 );

		// Cover window
		lout.dims.intDims[0] = 0;
		lout.dims.floatDims[1] = SB_CENTER_DIM( 0.45f );
		lout.dims.floatDims[2] = 0.5f;
		lout.dims.floatDims[3] = 0.75f;
		lout.type = SB_DIMTYPE_IFFF_TL_SRNN;

		wchar_t path[ MAX_PATH ] = { 0 };
		swprintf( path, MAX_PATH, L"Covers\\%s\\cover.jpg", n );

		coverImgWnd = SBRestrictedImageWindows.create( L"coverImgWnd", &lout, path );

		// info box
		lout.type = SB_DIMTYPE_IFFF_TR_SRNN;

		infoBox = SBBasicTextWindows.create( L"infoBox", &lout, 15 );

		// Title/artist/tags
		{
			wchar_t msg[4096] = { 0 };

			swprintf( msg, 4096, L"Title: %s", dld->title );
			SBBasicTextWindows.draw( infoBox, wcsdup( msg ) );
			ZeroMemory( msg, 4096 );

			swprintf( msg, 4096, L"Artist: %s", dld->author );
			SBBasicTextWindows.draw( infoBox, wcsdup( msg ) );
			ZeroMemory( msg, 4096 );

			swprintf( msg, 4096, L"Tags: %s", dld->tags );
			SBBasicTextWindows.draw( infoBox, wcsdup( msg ) );

		}

		// Progress string
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.intDims[1] = 50;

		progStr = SBStringWindows.create(
			L"progStr", &lout,
			L"Progress", 15, 0
		);

		// Progress bar
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.intDims[1] = 25;
		lout.dims.floatDims[2] = 0.8f;
		lout.dims.intDims[3] = 18;
		lout.type = SB_DIMTYPE_FIFI_BL_SRNN;

		progBar = SBProgressBarWindows.create( L"progBar", &lout, dld->tPgs );

		// Progress message
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.intDims[1] = 0;

		wchar_t progmsg[48] = { 0 };
		swprintf( progmsg, 48, L"Downloading: 1 of %u ~ Number %s", dld->tPgs, n );

		progMsg = SBStringWindows.create( L"progMsg", &lout, progmsg, 15, 0 );

		// Add all to window lists
		Lists.Add( ses->windows[i], numWnd );
		Lists.Add( ses->windows[i], coverImgWnd );
		Lists.Add( ses->windows[i], infoBox );
		Lists.Add( ses->windows[i], progStr );
		Lists.Add( ses->windows[i], progBar );
		Lists.Add( ses->windows[i], progMsg );
	}
	SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );
}

void StartPermDownloads_SbCommand_Download_Ses( sbCmd_Download_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		struct pDownloadThreadInfo *inf = newptr( struct pDownloadThreadInfo );
		sbWnd
			PROGBARWND *progBar = Lists.Get( ses->windows[i], 4 ),
			STRWND *progMsg = Lists.Get( ses->windows[i], 5 );
		sb_PermDownload *dld = ses->pDownloads + i;

		inf->dld = dld;
		inf->progressBar = progBar;
		inf->progressString = progMsg;

		uintptr_t th = _beginthread( PDownloads.finish, 0, inf );
		Lists.Add( SaucebotMaster.threadHandleList, ( void * )th );
	}
}

void End_SbCommand_Download_Ses( sbCmd_Download_Session *ses ) {
	for ( uint16_t i = 0; i < Lists.Size( ses->numList ); ++i ) {
		sbList *windowList = ses->windows[i];
		for ( uint16_t j = 0; j < Lists.Size( windowList ); ++j ) {
			sbWnd *wnd = Lists.Get( windowList, j );
			SBWindows.destroy( wnd );
		}
		Lists.Delete( windowList );
	}
	
	free( ses->windows );
	Lists.Delete( ses->numList );
	free( ses->fscreen );
	free( ses->nfscreen );
	SBWindows.destroy( ses->sessionParent );

	// Signal dowloader threads to end
	for ( uint16_t i = 0; i < Lists.Size( SaucebotMaster.threadHandleList ); ++i )
		ses->pDownloads[i].signal = 1;

	for ( uint16_t i = 0; i < Lists.Size( SaucebotMaster.threadHandleList ); ++i ) {
		_cwait( NULL, ( intptr_t )Lists.Get( SaucebotMaster.threadHandleList, i ), 0 );
		Lists.Remove( SaucebotMaster.threadHandleList, i-- );
	}

	Files.recursiveDelete( L"Covers" );
	free( ses->pDownloads );
	SaucebotMaster.cmdOn = 0;
}

void sbCmd_Chconf( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {

	// caret positioning doesn't work with only this string
	// kill me
	SetFocus( GetHWND( viewer ) );

	SBBasicTextWindows.clear( viewer );
	SBBasicTextWindows.draw( logger, L"Opened configurator" );
	SaucebotMaster.cmdOn = 1;
	
	jmp_buf *jmp = newptr( jmp_buf );
	int v = setjmp( *jmp );

	sbWnd
		STRWND *confTitle,
		STRWND *pdldInf,
		TBOX *pdldTbox,
		STRWND *pdldEmsg,
		STRWND *confirm;
	sbWnd_Layout lout = { 0 };
	lout.parent = viewer;

	if ( !v ) {
		// Set up windows and call another msgloop, nothing has happened yet.

		// "Configuration Editor" message
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.type = SB_DIMTYPE_FIII_TL_SRNN;
		confTitle = SBStringWindows.create(
			L"confTitle", &lout,
			L"Configuration Editor", 15, 0
		);

		// "Permanant download path" message
		lout.dims.intDims[0] = 0;
		lout.dims.intDims[1] = SB_CENTER_DIM( 40 );
		pdldInf = SBStringWindows.create(
			L"pdldInf", &lout,
			L"Permanent download path: ", 15, 0
		);

		// Permanant download path textbox

		lout.dims.intDims[0] = 1;
		lout.dims.intDims[1] = 0;
		lout.dims.floatDims[2] = 0.5f;
		lout.dims.intDims[3] = 19;
		lout.type = SB_DIMTYPE_IIFI_TL_SRTR;
		lout.sibling = pdldInf;

		pdldTbox = SBTextboxes.create( L"pdldTbox", &lout, 1 );

		wchar_t *path = EngineOps.DldCmd.getPermDldPath( );
		SBTextboxes.sendString( pdldTbox, path );
		free( path );

		// Error message for downloads
		lout.dims.floatDims[0] = SB_CENTER_DIM( 0.5f );
		lout.dims.intDims[1] = 60;
		lout.type = SB_DIMTYPE_FIII_TL_SRNN;
		lout.sibling = NULL;

		SBWindows.setCreateMode( SBWND_CREATEMODE_HIDE );
		pdldEmsg = SBStringWindows.create(
			L"pdldEmsg", &lout,
			L"placeholder", 15, 0
		);
		SBWindows.setCreateMode( SBWND_CREATEMODE_SHOW );

		// Confirm button
		lout.dims.intDims[0] = 0;
		lout.dims.intDims[1] = 0;
		lout.type = SB_DIMTYPE_IIII_BR_SRNN;

		confirm = SBStringWindows.create(
			L"confirm", &lout,
			L"Confirm", 15, 1
		);

		// Reference appending and function binding
		SBWindows.appendReference( confirm, jmp );
		SBWindows.appendReference( confirm, pdldTbox );
		SBWindows.appendReference( confirm, pdldEmsg );
		SBWindows.setSignalFn( confirm, Commands.Chconf.Confirm );

		msgloop( );
	} else {
		// setjmp returned again, destroy windows and exit
		SBWindows.destroy( confTitle );
		SBWindows.destroy( pdldInf );
		SBWindows.destroy( pdldTbox );
		SBWindows.destroy( pdldEmsg );
		SBWindows.destroy( confirm );
	}
	return;
}

void Confirm_SbCmd_Chconf( sbWnd *button, void *unused ) {
	// textbox is ref 1
	sbWnd *tbox = SBWindows.getReference( button, 1 );
	// emsg is ref 2
	sbWnd *emsg = SBWindows.getReference( button, 2 );

	wchar_t *path = SBTextboxes.getString( tbox );
	int res = Files.createAbsPath( path );
	switch ( res ) {
		case 1:
			SBStringWindows.changeString(
				emsg,
				L"Drive letter is invalid, enter a different drive letter."
			);
			SBWindows.show( emsg, 1 );
			return;
		case 2: {
			wchar_t msg[ MAX_PATH + 64 ] = { 0 };
			swprintf(
				msg,
				( MAX_PATH + 64 ),
				L"Could not create path %s, enter another path",
				( wchar_t * )path
			);
			SBStringWindows.changeString( emsg, msg );
			SBWindows.show( emsg, 1 );
			return;
		}
	}

	{
		FILE *f = _wfopen( L"config", L"w" );
		fwprintf( f, L"permdldpath=\"%s\"\n", path );
		fclose( f );
	}

	SaucebotMaster.cmdOn = 0;
	// jmp_buf is ref 0
	jmp_buf *jmp = SBWindows.getReference( button, 0 );
	longjmp( *jmp, 1 );

	return;
}

void sbCmd_Search( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	/*
	 * Drafting for search command
	 * 
	 */
	return;
}

void sbCmd_Preset( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	return;
}



void sbCmd_Info( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	// Doesn't set cmdOn, as there is no action the user can take. However, this returns if cmdOn is set.
	if ( SaucebotMaster.cmdOn ) {
		SBBasicTextWindows.draw( logger, L"Cannot execute command" );
		return;
	}
	SBBasicTextWindows.draw( logger, L"Requested info" );
	
	SBBasicTextWindows.clear( viewer );
	SBBasicTextWindows.draw( viewer, L"Saucebot is an nhentai client app with a GUI controlled (partly) by a command line." );
	SBBasicTextWindows.draw( viewer, L"Currently, it has reached version 0.3 \"エッチ\" (ecchi)." );
	SBBasicTextWindows.draw( viewer, L"There are seven commands, of which five are currently implemented." );
	SBBasicTextWindows.draw( viewer, L"Usages:" );
	SBBasicTextWindows.draw( viewer, L"1. Number:" );
	SBBasicTextWindows.draw( viewer, L"    number [n1] [n2] ... [nx]" );
	SBBasicTextWindows.draw( viewer, L"    Opens the numbers specified. Any number can also be 'rand', for a random number." );
	SBBasicTextWindows.draw( viewer, L"2. Download:" );
	SBBasicTextWindows.draw( viewer, L"    download [n1] [n2] ... [nx]" );
	SBBasicTextWindows.draw( viewer, L"    Downloads the numbers specified into a permanant download location." );
	SBBasicTextWindows.draw( viewer, L"    saucebot will prompt you for a location." );
	SBBasicTextWindows.draw( viewer, L"    Enter a path beginning with a drive letter." );
	SBBasicTextWindows.draw( viewer, L"    So long as there are numbers in the path specified, the number command will be aware of them." );
	SBBasicTextWindows.draw( viewer, L"    Your username is located at C:\\Users\\[name]." );
	SBBasicTextWindows.draw( viewer, L"3. Chconf:" );
	SBBasicTextWindows.draw( viewer, L"    chconf" );
	SBBasicTextWindows.draw( viewer, L"    Opens saucebot's config editor." );
	SBBasicTextWindows.draw( viewer, L"4. Info:" );
	SBBasicTextWindows.draw( viewer, L"    info" );
	SBBasicTextWindows.draw( viewer, L"    Displays this information." );
	SBBasicTextWindows.draw( viewer, L"5. Exit:" );
	SBBasicTextWindows.draw( viewer, L"    exit" );
	SBBasicTextWindows.draw( viewer, L"    Exits saucebot." );
	SBBasicTextWindows.draw( viewer, L"Unimplemented Commands:" );
	SBBasicTextWindows.draw( viewer, L"search: Search nhentai" );
	SBBasicTextWindows.draw( viewer, L"preset: preset search combinations" );
	return;
}

void sbCmd_Exit( sbWnd *viewer, sbWnd *logger, wchar_t *text ) {
	exit( EXIT_SUCCESS );
	return;
}