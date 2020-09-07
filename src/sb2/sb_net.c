#include <stdio.h>
#include <windows.h>
#include <winhttp.h>

#include "sb2/sb_net.h"
#include "sb2/sb_init.h"

SbNetFnStruct SbNet;

#define newarr(type,num)	calloc(num,sizeof(type))

// #define PRINT_ERROR

void initNet( void ) {
	SbNet.numget = numget_winhttp;
	SbNet.imgget = imgget_winhttp;
	SbNet.imgget_tsite = imgget_tsite_winhttp;
}

// TODO: Replace with WinHttp
// Done!

void numget_winhttp( wchar_t *num, wchar_t *path ) {
	wchar_t request[48] = { 0 };
	// num = "number/page" or just "number", add /g/ to the beginning for request string.
	swprintf( request, 48, L"/g/%ls", num );

	HINTERNET req = WinHttpOpenRequest( SaucebotMaster.conSite, L"GET", request, NULL, NULL, NULL, WINHTTP_FLAG_SECURE );
	WinHttpSendRequest( req, NULL, 0, NULL, 0, 0, 0 );
	WinHttpReceiveResponse( req, NULL );
	
	unsigned long sz = 0;
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &sz, NULL );
	wchar_t *buf = newarr( wchar_t, sz );
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, buf, &sz, NULL );
	if ( wcscmp( buf, L"200" ) ) {

		#ifdef PRINT_ERROR
		printf( "Http error: status code %ls\n", buf );
		#endif

		return;
	}
	free( buf );

	FILE *f = _wfopen( path, L"w" );
	unsigned long ct = 0;
	unsigned long res = 0;
	do {
		WinHttpQueryDataAvailable( req, &ct );
		char *buf = newarr( char, ct );
		WinHttpReadData( req, buf, ct, &res );
		fwrite( buf, 1, ct, f );
		ct = 0;
		free( buf );
	} while ( res > 0 );

	fclose( f );
	WinHttpCloseHandle( req );
}

void imgget_winhttp( char *url, wchar_t *path ) {
	wchar_t *request = newarr( wchar_t, strlen( url ) );
	sscanf( url, "https://i.nhentai.net%ls", request );

	HINTERNET req = WinHttpOpenRequest( SaucebotMaster.conImages, L"GET", request, NULL, NULL, NULL, WINHTTP_FLAG_SECURE );
	WinHttpSendRequest( req, NULL, 0, NULL, 0, 0, 0 );
	WinHttpReceiveResponse( req, NULL );
	
	unsigned long sz = 0;
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &sz, NULL );
	wchar_t *buf = newarr( wchar_t, sz );
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, buf, &sz, NULL );
	if ( wcscmp( buf, L"200" ) ) {

		#ifdef PRINT_ERROR
		printf( "Http error: status code %ls\n", buf );
		#endif

		return;
	}
	free( buf );

	FILE *f = _wfopen( path, L"wb" );
	unsigned long ct = 0;
	unsigned long res = 0;
	do {
		WinHttpQueryDataAvailable( req, &ct );
		char *buf = newarr( char, ct );
		WinHttpReadData( req, buf, ct, &res );
		fwrite( buf, 1, ct, f );
		ct = 0;
		free( buf );
	} while ( res > 0 );

	fclose( f );
	WinHttpCloseHandle( req );
}

void imgget_tsite_winhttp( wchar_t *url, wchar_t *path ) {
	wchar_t *request = newarr( wchar_t, wcslen( url ) );
	swscanf( url, L"https://t.nhentai.net%s", request );

	HINTERNET req = WinHttpOpenRequest( SaucebotMaster.conCovers, L"GET", request, NULL, NULL, NULL, WINHTTP_FLAG_SECURE );
	WinHttpSendRequest( req, NULL, 0, NULL, 0, 0, 0 );
	WinHttpReceiveResponse( req, NULL );
	
	unsigned long sz = 0;
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, NULL, &sz, NULL );
	wchar_t *buf = newarr( wchar_t, sz );
	WinHttpQueryHeaders( req, WINHTTP_QUERY_STATUS_CODE, NULL, buf, &sz, NULL );
	if ( wcscmp( buf, L"200" ) ) {

		#ifdef PRINT_ERROR
		printf( "Http error: status code %ls\n", buf );
		#endif

		return;
	}
	free( buf );

	FILE *f = _wfopen( path, L"wb" );
	unsigned long ct = 0;
	unsigned long res = 0;
	do {
		WinHttpQueryDataAvailable( req, &ct );
		char *buf = newarr( char, ct );
		WinHttpReadData( req, buf, ct, &res );
		fwrite( buf, 1, ct, f );
		ct = 0;
		free( buf );
	} while ( res > 0 );

	fclose( f );
	WinHttpCloseHandle( req );
}