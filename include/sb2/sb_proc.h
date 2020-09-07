#ifndef SB_PROC_MODULE
#define SB_PROC_MODULE

#include "../sbwnd/sbgui.h"

void initEvents( void );


// Number command
void numCmdProc( sbWnd *, void * );

typedef struct {

	void ( *prevNum )( sbWnd *, void * );

	void ( *nextNum )( sbWnd *, void * );

	void ( *nextPage )( sbWnd *, void * );

	void ( *prevPage )( sbWnd *, void * );

	void ( *close )( sbWnd *, void * );

	void ( *fullscreen )( sbWnd *, void * );

} SbNumCmdEventFnStruct;

extern SbNumCmdEventFnStruct SbNumCmdEvent;

void prevNum_NumCmdEvent( sbWnd *, void * );
void nextNum_NumCmdEvent( sbWnd *, void * );
void nextPage_NumCmdEvent( sbWnd *, void * );
void prevPage_NumCmdEvent( sbWnd *, void * );
void close_NumCmdEvent( sbWnd *, void * );
void fullscreen_NumCmdEvent( sbWnd *, void * );

#define SBEVT_PREVNUM   	(0x10)
#define SBEVT_NEXTNUM   	(0x11)
#define SBEVT_PREVPAGE  	(0x12)
#define SBEVT_NEXTPAGE  	(0x13)
#define SBEVT_CLOSE			(0x14)
#define SBEVT_FULLSCREEN	(0x15)

#define SbNumCmdEvents    SbNumCmdEvent


// Download command
void dldCmdProc( sbWnd *, void * );

typedef struct {

	void ( *prevNum )( sbWnd *, void * );

	void ( *nextNum )( sbWnd *, void * );

	void ( *fullscreen )( sbWnd *, void * );

	void ( *close )( sbWnd *, void * );

} SbDldCmdEvenFnStruct;

extern SbDldCmdEvenFnStruct SbDldCmdEvent;

void prevNum_DldCmdEvent( sbWnd *, void * );
void nextNum_DldCmdEvent( sbWnd *, void * );
void fullscreen_DldCmdEvent( sbWnd *, void * );
void close_DldCmdEvent( sbWnd *, void * );

#define SBEVT_D_PREVNUM		(0x20)
#define SBEVT_D_NEXTNUM		(0x21)
#define SBEVT_D_FULLSCREEN	(0x23)
#define SBEVT_D_CLOSE		(0x24)

#define SbDldCmdEvents		SbDldCmdEvent

#endif