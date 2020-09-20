#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <winhttp.h>

#include "sb2/sb_init.h"
#include "sb2/sb_eng.h"
#include "sb2/sb_file.h"
#include "sb2/sb_proc.h"
#include "sb2/sb_net.h"
#include "sb2/sb_rand.h"

SbMasterStruct SaucebotMaster;

#define PRINT_ERROR

void initSB( void ) {

	cmdFuncs[0] = sbCmd_Number;
	cmdFuncs[1] = sbCmd_Search;
	cmdFuncs[2] = sbCmd_Preset;
	cmdFuncs[3] = sbCmd_Download;
	cmdFuncs[4] = sbCmd_Info;
	cmdFuncs[5] = sbCmd_Exit;
	cmdFuncs[6] = sbCmd_Chconf;
	SaucebotMaster.hses = WinHttpOpen( L"Saucebot 0.3", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0 );
	SaucebotMaster.conSite = WinHttpConnect( SaucebotMaster.hses, L"nhentai.net", INTERNET_DEFAULT_HTTPS_PORT, 0 );
	SaucebotMaster.conImages = WinHttpConnect( SaucebotMaster.hses, L"i.nhentai.net", INTERNET_DEFAULT_HTTPS_PORT, 0 );
	SaucebotMaster.conCovers = WinHttpConnect( SaucebotMaster.hses, L"t.nhentai.net", INTERNET_DEFAULT_HTTPS_PORT, 0 );

	#ifdef RELEASE
	FreeConsole( );
	#endif

	initNet( );
	initSbFile( );
	initEngine( );
	initEvents( );
	initRandom( );

	GetCurrentDirectoryW( MAX_PATH, SaucebotMaster.startDir );
	wchar_t *path = Files.getAppDataPath( );
	GetFileAttributesW( path );
	if ( GetLastError( ) == ERROR_FILE_NOT_FOUND )
		CreateDirectoryW( path, NULL );
	SetCurrentDirectoryW( path );

	if ( GetFileAttributesW( L"TempDownloads" ) != INVALID_FILE_ATTRIBUTES )
		Files.recursiveDelete( L"TempDownloads" );

	if ( GetFileAttributesW( L"Covers" ) != INVALID_FILE_ATTRIBUTES )
		Files.recursiveDelete( L"Covers" );

	GetFileAttributesW( L"temp.txt" );
	if ( GetLastError( ) != ERROR_FILE_NOT_FOUND )
		DeleteFileW( L"temp.txt" );

	FILE *f = _wfopen( L"config", L"a" );
	fclose( f );

	_beginthread( initSB_conTest, 0, NULL );

	SaucebotMaster.threadHandleList = Lists.New( );
	free( path );
}

void initSB_conTest( void *unused ) {
	/*
	 * Connection test to nhentai.net and i.nhentai.net: exit on failiure.
	 * Also gets the current maximum number.
	 */

	HINTERNET req = WinHttpOpenRequest( SaucebotMaster.conSite, L"GET", L"/", NULL, NULL, NULL, WINHTTP_FLAG_SECURE );
	// Use /galleries/254/1.jpg because it can return a 200 and not a 403.
	HINTERNET imgReq = WinHttpOpenRequest( SaucebotMaster.conImages, L"GET", L"/galleries/254/1.jpg", NULL, NULL, NULL, WINHTTP_FLAG_SECURE );
	HINTERNET coverReq = WinHttpOpenRequest( SaucebotMaster.conCovers, L"GET", L"/galleries/254/cover.jpg", NULL, NULL, NULL, WINHTTP_FLAG_SECURE );

	WinHttpSendRequest( req, NULL, 0, NULL, 0, 0, 0 );
	WinHttpSendRequest( imgReq, NULL, 0, NULL, 0, 0, 0 );
	WinHttpSendRequest( coverReq, NULL, 0, NULL, 0, 0, 0 );

	WinHttpReceiveResponse( req, NULL );
	WinHttpReceiveResponse( imgReq, NULL );
	WinHttpReceiveResponse( coverReq, NULL );

	unsigned long sz = 0, imgSz = 0, coverSz = 0;
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &sz, NULL );
	WinHttpQueryHeaders( imgReq, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &imgSz, NULL );
	WinHttpQueryHeaders( coverReq, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &coverSz, NULL );

	wchar_t *buf = calloc( sz, 2 );
	wchar_t *imgBuf = calloc( imgSz, 2 );
	wchar_t *coverBuf = calloc( coverSz, 2 );

	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, buf, &sz, NULL );
	WinHttpQueryHeaders( imgReq, WINHTTP_QUERY_STATUS_CODE, NULL, imgBuf, &imgSz, NULL );
	WinHttpQueryHeaders( coverReq, WINHTTP_QUERY_STATUS_CODE, NULL, coverBuf, &coverSz, NULL );

	wchar_t *httpOk = L"200";
	if ( wcscmp( buf, httpOk ) ) {
		wchar_t msg[128] = { 0 };
		swprintf( msg, 128, L"Failure to connect to nhentai.net: status code %ls. Offline mode enabled.", buf );
		MessageBoxW( NULL, msg, L"Connection Failure", MB_OK | MB_ICONEXCLAMATION );
		SaucebotMaster.offlineMode = 1;
	}

	if ( wcscmp( imgBuf, httpOk ) ) {
		wchar_t msg[128] = { 0 };
		swprintf( msg, 128, L"Failure to connect to i.nhentai.net: status code %ls. Offline mode enabled.", imgBuf );
		MessageBoxW( NULL, msg, L"Connection Failure", MB_OK | MB_ICONEXCLAMATION );
		SaucebotMaster.offlineMode = 1;
	}

	if ( wcscmp( coverBuf, httpOk ) ) {
		wchar_t msg[128] = { 0 };
		swprintf( msg, 128, L"Failure to connect to t.nhentai.net: status code %ls. Offline mode enabled.", coverBuf );
		MessageBoxW( NULL, msg, L"Connection Failure", MB_OK | MB_ICONEXCLAMATION );
		SaucebotMaster.offlineMode = 1;
	}

	WinHttpCloseHandle( req );
	WinHttpCloseHandle( imgReq );
	WinHttpCloseHandle( coverReq );

	// Get max number
	SbNet.numget( L"..", L"temp.txt" ); // numget uses "/g/%s", so use ".." to reverse this.
	FILE *f = _wfopen( L"temp.txt", L"r" );

	HTML_ops.seekString( f, L"New Uploads" );
	HTML_ops.seekString( f, L"a href=\"/" );
	wchar_t *n = HTML_ops.readBetween( f, L'/', L'/' );
	swscanf( n, L"%u", &SaucebotMaster.nMax );

	fclose( f );

	/*
	 * End connection Test
	 */
	return;
}

void shutdownSB2( void ) {
	uint16_t i;
	for ( i = 0; i < Lists.Size( SaucebotMaster.threadHandleList ); ++i ) {
		WINBOOL c = TerminateThread( Lists.Get( SaucebotMaster.threadHandleList, i ), 0 );
		#ifdef PRINT_ERROR
		if ( !c )
			printf( "Error Terminating Downloader Thread: %ld\n", GetLastError( ) );
		#endif
	}
	
}