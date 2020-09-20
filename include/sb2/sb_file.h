#ifndef SB_FILE_MODULE
#define SB_FILE_MODULE

// Header for saucebot's file operations.

#include <stdio.h>
#include <stdint.h>


typedef struct {
	struct {
		// AppData/Local/Saucebot path for this machine. Has the '/' appended to the end by default.
		wchar_t *( *getAppDataPath )( void );

		void ( *recursiveDelete )( wchar_t * );
		int ( *copyDir )( wchar_t *, wchar_t * );
		void ( *readString )( FILE *, wchar_t *, size_t );

		// Tries to create an absolute path. Returns 1 or 2 on failiure.
		// Returns 1 if the drive letter is bad.
		// Returns 2 if the path can't be created.
		int ( *createAbsPath )( wchar_t * );

	} File;
	
	struct {
		// Seeks to after a string in a file. Returns 1 on failure.
		uint8_t ( *seekString )( FILE *, wchar_t *);
		wchar_t *( *readBetween )( FILE *, wchar_t, wchar_t );
	} HTML;
} SbFileFnStruct;

extern SbFileFnStruct SbFile;

#define Files		SbFile.File
#define HTML_ops	SbFile.HTML

void initSbFile( void );

wchar_t *sbFile_GetAppDataPath( void );
void sbFile_ReadString( FILE *, wchar_t *, size_t );
int sbFile_copyDir( wchar_t *, wchar_t * );
void sbFile_RecursiveDelete( wchar_t * );
int sbFile_createAbsPath( wchar_t * );

uint8_t sbHTML_SeekString( FILE *, wchar_t * );
wchar_t *sbHTML_ReadBetween( FILE *, wchar_t, wchar_t );

#endif