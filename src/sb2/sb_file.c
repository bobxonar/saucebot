#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "sb2/sb_file.h"

#define newwstr( len )	calloc(len,2)

// #define PRINT_ERROR

SbFileFnStruct SbFile;

void initSbFile( void ) {
	Files.getAppDataPath = sbFile_GetAppDataPath;
	Files.recursiveDelete = sbFile_RecursiveDelete;
	Files.readString = sbFile_ReadString;
	Files.createAbsPath = sbFile_createAbsPath;
	HTML_ops.readBetween = sbHTML_ReadBetween;
	HTML_ops.seekString = sbHTML_SeekString;
}

wchar_t *sbFile_GetAppDataPath( void ) {
	wchar_t userN[ 256 ] = { 0 };
	DWORD sz = 256;
	GetUserNameW( userN, &sz );
	wchar_t *path = calloc( MAX_PATH, sizeof( wchar_t ) );
	wsprintfW( path, L"C:\\Users\\%ls\\AppData\\Local\\Saucebot\\", userN );
	
	return path;
}


void sbFile_RecursiveDelete( wchar_t *folder ) {
	SetLastError( 0 );
	wchar_t path[ MAX_PATH ] = { 0 };

	wchar_t current[ MAX_PATH ] = { 0 };
	swprintf( current, MAX_PATH, L"%ls\\*", folder );
	
	WIN32_FIND_DATAW findData = { 0 };
	HANDLE session = FindFirstFileW( current, &findData );
	while ( GetLastError( ) != ERROR_NO_MORE_FILES ) {
		if ( ( !wcscmp( findData.cFileName, L"." ) ) || ( !wcscmp( findData.cFileName, L".." ) ) )
			goto next;

		wchar_t filename[ MAX_PATH ];
		swprintf( filename, MAX_PATH, L"%ls\\%ls", folder, findData.cFileName );

		if ( GetFileAttributesW( filename ) & FILE_ATTRIBUTE_DIRECTORY )
			sbFile_RecursiveDelete( filename );

		DeleteFileW( filename );

		#ifdef PRINT_ERROR
		if ( GetLastError( ) != 0 )
			printf( "Error Removing File with name %ls: %ld\n", filename, GetLastError( ) );
		#endif

		next:
		FindNextFileW( session, &findData );
	}

	FindClose( session );
	RemoveDirectoryW( folder );

	#ifdef PRINT_ERROR
	if ( GetLastError( ) != 0 )
		printf( "Error removing directory: %ld\n", GetLastError( ) );
	#endif
}

uint8_t sbHTML_SeekString( FILE *fle, wchar_t *str ) {
	int ch;
	
	while ( ( ch = fgetc( fle ) ) != EOF ) {
		if ( ch == str[0] ) {
			size_t l = wcslen( str );
			wchar_t *compStr = newwstr( l );
			fseek( fle, -1, SEEK_CUR );
			Files.readString( fle, compStr, l );
			if ( !wcscmp( str, compStr ) )
				return 0;
			free( compStr );
			long seekl = l - 1;
			fseek( fle, -seekl, SEEK_CUR );
		}
	}
	return 1;
}

// Assumes the buffer is large enough.
void sbFile_ReadString( FILE *fle, wchar_t *buf, size_t len ) {
	for ( size_t i = 0; i < len; i++ )
		buf[i] = fgetc( fle );
	return;
}

int sbFile_createAbsPath( wchar_t *path ) {
	size_t l = wcslen( path );
	wchar_t
		driveLetter[4] = { 0 },
		*pathr = wcsdup( path ), // wcstok modifies input string, need this
		*toCons = newwstr( ( l + 1 ) ),
		*toCons_ptr = toCons;

	{
		swscanf( path, L"%2[^/\\]", driveLetter );
		driveLetter[ wcslen( driveLetter ) ] = '\\'; // Append backslash
		unsigned a = GetDriveTypeW( driveLetter );
		if ( a < 2 )
			return 1;
	}

	wchar_t *curFolder = wcstok( pathr, L"/\\" );
	{
		do {
			swprintf( toCons_ptr, l, L"%s\\", curFolder );
			toCons_ptr += wcslen( curFolder ) + 1;

			CreateDirectoryW( toCons, NULL );
			if ( GetFileAttributesW( toCons ) == INVALID_FILE_ATTRIBUTES ) {
				return 2;
			}
			curFolder = wcstok( NULL, L"/\\" );
		} while ( curFolder != NULL );
	}
	return 0;
}

wchar_t *sbHTML_ReadBetween( FILE *fle, wchar_t beginCh, wchar_t endCh ) {
	// Count characters first
	uint8_t items = 0;
	size_t len = 0;
	int ch = 0;
	while ( items < 2 && ch != EOF) {
		ch = fgetc( fle );
		if ( ch == beginCh && items != 1 ) {
			items = 1;
			continue;
		}
		if ( ch == endCh ) {
			items = 2;
			continue;
		}
		if ( items == 1 )
			len++;
	}

	fseek( fle, -( len + 1 ), SEEK_CUR );
	wchar_t *fin = newwstr( len + 1 );
	for ( size_t i = 0; i < len; ++i )
		fin[i] = fgetc( fle );
	return fin;
}