#ifndef SB_ENG_MODULE
#define SB_ENG_MODULE

#include "../sbwnd/sbgui.h"
#include <wchar.h>
#include <windows.h>

// This is where the magic happens.

typedef void ( *CommandFunc )( sbWnd *, sbWnd *, wchar_t * );

extern CommandFunc cmdFuncs[7];

typedef enum cmdtype {
	NUMBER = 0,
	SEARCH,
	PRESET,
	DOWNLOAD,
	INFO,
	EXIT,
	CHCONF,
	UNKNOWN = 255,
} cmdType;

cmdType sbCmd_GetType( wchar_t * );

typedef struct tempdownload {
	uint16_t tPgs;
	uint16_t nPgs;
	wchar_t number[12];
	wchar_t gallery[32];
	int signal; // Signal to terminate thread
} sb_TempDownload;

typedef struct permdownload {
	uint16_t
		tPgs,
		nPgs;

	wchar_t
		number[12],
		gallery[32],
		title[1024],
		author[256],
		tags[1024],
		path[ MAX_PATH ];

	int signal;
} sb_PermDownload;

typedef struct sbcmddldsession {
	sbWnd *sessionParent;
	sbList 
		*numList,
		**windows;

	sbWnd_Dims
		*fscreen,
		*nfscreen;

	uint8_t
		fscreenType,
		nfscreenType;

	int fscreenOn;
	uint16_t idx;
	sb_PermDownload *pDownloads;
} sbCmd_Download_Session;

typedef struct {

	struct {

		struct sbcmdnumsession *( *Begin )( sbWnd *, sbList * );

		void ( *CreateWindows )( struct sbcmdnumsession * );

		void ( *End )( struct sbcmdnumsession * );

	} Number;

	struct {

		// Checks if a path for the permanant download path exists. Returns a nonzero value if there is no path.
		int ( *CheckConfig )( void );

		// Textbox out function for the config prompt. Reference 0 will be a jmp_buf that can be used to jump back into the command.
		void ( *StorePath )( sbWnd *, void * );

		// Closes command through path enterer. Ref 0 is jump
		void ( *Close )( sbWnd *, void * );

		struct {

			sbCmd_Download_Session *( *Begin )( sbWnd *, sbList * );

			void ( *InitCoverFolders )( sbCmd_Download_Session * );

			void ( *CreateStaticWindows )( sbCmd_Download_Session * );

			void ( *CreateDynamicWindows )( sbCmd_Download_Session * );

			void ( *StartPermDownloads )( sbCmd_Download_Session * );

			void ( *End )( sbCmd_Download_Session * );

		} Ses;

	} Download;

	struct {

		void ( *Confirm )( sbWnd *, void * );

	} Chconf;

} SbCommandFnStruct;

extern SbCommandFnStruct SbCommand;

typedef struct {
	
	struct {

		wchar_t *( *getImagePath )( wchar_t *, uint16_t );

		void ( *updateWindows )( struct sbcmdnumsession * );

		void ( *download )( struct sbcmdnumsession * );

		void ( *showWindows )( struct sbcmdnumsession * );

	} NumCmd;

	struct {

		void ( *create )( sbList * );
		
		void ( *init )( sb_TempDownload *, sbList * );

		void ( *advance )( sb_TempDownload * );

		void ( *finish )( void * );

	} TDownload;

	struct {

		wchar_t *( *getPermDldPath )( void );

		void ( *remDupNumbers )( sbList * );

		void ( *updateWindows )( sbCmd_Download_Session * );

		void ( *showWindows )( sbCmd_Download_Session * );

	} DldCmd;

	struct {

		void ( *create )( sbList * );

		void ( *init )( sb_PermDownload *, sbList * );

		void ( *advance )( sb_PermDownload * );

		void ( *finish )( void * );

	} PDownload;

} SbEngineFnStruct;

extern SbEngineFnStruct SbEngine;


#define Commands    SbCommand
#define EngineOps   SbEngine
#define TDownloads	SbEngine.TDownload
#define PDownloads	SbEngine.PDownload

typedef struct sbcmdnumsession {
	sbWnd *sessionParent;
	sbList 
		*numList,
		*currentPgs,
		**windows;

	sbWnd_Dims
		*fscreen,
		*nfscreen;

	uint8_t
		fscreenType,
		nfscreenType;

	int fscreenOn;
	uint16_t idx;
	sb_TempDownload *tDownloads;
} sbCmd_Number_Session;

void initEngine( void );

void sbCmd_Number( sbWnd *, sbWnd *, wchar_t * );
sbCmd_Number_Session *Begin_SbCommand_Number( sbWnd *, sbList * );
void CreateWindows_SbCommand_Number( sbCmd_Number_Session * );
void End_SbCommand_Number( sbCmd_Number_Session * );

void create_SbEngine_TDownload( sbList * );
void init_SbEngine_TDownload( sb_TempDownload *, sbList * );
void advance_SbEngine_TDownload( sb_TempDownload * );
void finish_SbEngine_TDownload( void * );

wchar_t *getImagePath_SbEngine_NumCmd( wchar_t *, uint16_t );
void updateWindows_SbEngine_NumCmd( sbCmd_Number_Session * );
void download_SbEngine_NumCmd( sbCmd_Number_Session * );
void showWindows_SbEngine_NumCmd( sbCmd_Number_Session * );

void sbCmd_Chconf( sbWnd *, sbWnd *, wchar_t * );
void Confirm_SbCmd_Chconf( sbWnd *, void * );

void sbCmd_Search( sbWnd *, sbWnd *, wchar_t * );
void sbCmd_Preset( sbWnd *, sbWnd *, wchar_t * );

void sbCmd_Download( sbWnd *, sbWnd *, wchar_t * );
int CheckConfig_SbCommand_Download( void );
void StorePath_SbCommand_Download( sbWnd *, void * );
void Close_SbCommand_Download( sbWnd *, void * );

void create_SbEngine_PDownload( sbList * );
void init_SbEngine_PDownload( sb_PermDownload *, sbList * );
void advance_SbEngine_PDownload( sb_PermDownload * );
void finish_SbEngine_PDownload( void * );

wchar_t *getPermDldPath_SbEngine_DldCmd( void );
void remDupNumbers_SbEngine_DldCmd( sbList * );
void updateWindows_SbEngine_DldCmd( sbCmd_Download_Session * );
void showWindows_SbEngine_DldCmd( sbCmd_Download_Session * );

sbCmd_Download_Session *Begin_SbCommand_Download_Ses( sbWnd *, sbList * );
void InitCoverFolders_SbCommand_Download_Ses( sbCmd_Download_Session * );
void CreateStaticWindows_SbCommand_Download_Ses( sbCmd_Download_Session * );
void CreateDynamicWindows_SbCommand_Download_Ses( sbCmd_Download_Session * );
void StartPermDownloads_SbCommand_Download_Ses( sbCmd_Download_Session * );
void End_SbCommand_Download_Ses( sbCmd_Download_Session * );

void sbCmd_Info( sbWnd *, sbWnd *, wchar_t * );
void sbCmd_Exit( sbWnd *, sbWnd *, wchar_t * );

void sbEng_CreateTempDownloads( sbList * );

void sbEntry( sbWnd *, void * );

#endif