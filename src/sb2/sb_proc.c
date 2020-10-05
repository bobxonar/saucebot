#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#include "sbwnd/sbgui.h"
#include "sb2/sb2.h"

SbNumCmdEventFnStruct SbNumCmdEvent;
SbDldCmdEvenFnStruct SbDldCmdEvent;

void initEvents( void ) {
	SbNumCmdEvents.prevNum = prevNum_NumCmdEvent;
	SbNumCmdEvents.nextNum = nextNum_NumCmdEvent;
	SbNumCmdEvents.prevPage = prevPage_NumCmdEvent;
	SbNumCmdEvents.nextPage = nextPage_NumCmdEvent;
	SbNumCmdEvents.close = close_NumCmdEvent;
	SbNumCmdEvents.fullscreen = fullscreen_NumCmdEvent;

	SbDldCmdEvents.prevNum = prevNum_DldCmdEvent;
	SbDldCmdEvents.nextNum = nextNum_DldCmdEvent;
	SbDldCmdEvents.fullscreen = fullscreen_DldCmdEvent;
	SbDldCmdEvents.close = close_DldCmdEvent;
}

void prevNum_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_PREVNUM );
}

void nextNum_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_NEXTNUM );
}

void prevPage_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_PREVPAGE );
}

void nextPage_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_NEXTPAGE );
}

void close_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_CLOSE );
}

void fullscreen_NumCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sessionWindow = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sessionWindow, sessionWindow, ( void * )SBEVT_FULLSCREEN );
}

void numCmdProc( sbWnd *wnd, void *msg ) {
	uint64_t evt = ( uint64_t )msg;
	sbCmd_Number_Session *ses = SBWindows.getReference( wnd, 0 );

	switch( evt ) {

		case SBEVT_NEXTNUM: {
			if ( ( ses->idx + 1 ) >= Lists.Size( ses->numList ) )
				return;
			sbList *currentWindows = ses->windows[ ses->idx++ ];

			for ( uint16_t i = 0; i < Lists.Size( currentWindows ); ++i ) {
				sbWnd *w = Lists.Get( currentWindows, i );
				SBWindows.hide( w, 1 );
			}

			EngineOps.NumCmd.updateWindows( ses );
			EngineOps.NumCmd.showWindows( ses );

			break;
		}

		case SBEVT_PREVNUM: {
			if ( ses->idx == 0 )
				return;
			sbList *currentWindows = ses->windows[ ses->idx-- ];

			for ( uint16_t i = 0; i < Lists.Size( currentWindows ); ++i ) {
				sbWnd *w = Lists.Get( currentWindows, i );
				SBWindows.hide( w, 1 );
			}

			EngineOps.NumCmd.updateWindows( ses );
			EngineOps.NumCmd.showWindows( ses );
			break;
		}

		case SBEVT_NEXTPAGE: {
			sb_TempDownload *dld = ses->tDownloads + ses->idx;
			uint16_t pg = ( uint64_t )Lists.Get( ses->currentPgs, ses->idx );
			if ( pg >= dld->tPgs )
				return;

			uint64_t pg64 = pg;
			Lists.Set( ses->currentPgs, ses->idx, ( void * )( pg64 + 1 ) );

			EngineOps.NumCmd.updateWindows( ses );
			EngineOps.NumCmd.showWindows( ses );
			break;
		}

		case SBEVT_PREVPAGE: {
			uint16_t pg = ( uint64_t )Lists.Get( ses->currentPgs, ses->idx );
			if ( pg == 1 )
				return;

			uint64_t pg64 = pg;
			Lists.Set( ses->currentPgs, ses->idx, ( void * )( pg64 - 1 ) );

			EngineOps.NumCmd.updateWindows( ses );
			EngineOps.NumCmd.showWindows( ses );
			break;
		}

		case SBEVT_FULLSCREEN: {
			ses->fscreenOn = !ses->fscreenOn;
			if ( ses->fscreenOn ) {
				sbWnd *parent = SBWindows.getParent( wnd );
				SBWindows.changeDims( parent, ses->fscreenType, ses->fscreen );
				SBWindows.toSurface( parent );
			} else
				SBWindows.changeDims( SBWindows.getParent( wnd ), ses->nfscreenType, ses->nfscreen );
			
			EngineOps.NumCmd.showWindows( ses );
			break;
		}

		case SBEVT_CLOSE: {
			sbWnd *p = SBWindows.getParent( wnd );
			sbWnd_Dims d = { 0 };
			uint8_t dt = ses->nfscreenType;
			memcpy( &d, ses->nfscreen, sizeof( sbWnd_Dims ) );
			Commands.Number.End( ses );
			if ( ses->fscreenOn )
				SBWindows.changeDims( p, dt, &d );
			break;
		}
	}
	return;
}

void prevNum_DldCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sesWnd = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sesWnd, sesWnd, ( void * )SBEVT_D_PREVNUM );
}

void nextNum_DldCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sesWnd = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sesWnd, sesWnd, ( void * )SBEVT_D_NEXTNUM );
}

void fullscreen_DldCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sesWnd = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sesWnd, sesWnd, ( void * )SBEVT_D_FULLSCREEN );
}

void close_DldCmdEvent( sbWnd *wnd, void *data ) {
	sbWnd *sesWnd = SBWindows.getReference( wnd, 0 );
	SBWindows.signalFn( sesWnd, sesWnd, ( void * )SBEVT_D_CLOSE );
}

void dldCmdProc( sbWnd *wnd, void *msg ) {
	uint64_t evt = ( uint64_t )msg;
	sbCmd_Download_Session *ses = SBWindows.getReference( wnd, 0 );

	switch( evt ) {

		case SBEVT_D_PREVNUM: {
			if ( ses->idx == 0 )
				return;

			ses->idx--;
			EngineOps.DldCmd.showWindows( ses );
			
			break;
		}

		case SBEVT_D_NEXTNUM: {
			if ( ( ses->idx + 1 ) >= Lists.Size( ses->numList ) )
				return;

			ses->idx++;
			EngineOps.DldCmd.showWindows( ses );

			break;
		}

		case SBEVT_D_FULLSCREEN: {
			ses->fscreenOn = !ses->fscreenOn;
			if ( ses->fscreenOn ) {
				sbWnd *parent = SBWindows.getParent( wnd );
				SBWindows.changeDims( parent, ses->fscreenType, ses->fscreen );
				SBWindows.toSurface( parent );
			} else
				SBWindows.changeDims( SBWindows.getParent( wnd ), ses->nfscreenType, ses->nfscreen );
			
			EngineOps.DldCmd.showWindows( ses );
			break;
		}

		case SBEVT_D_CLOSE: {
			sbWnd *p = SBWindows.getParent( wnd );
			sbWnd_Dims d = { 0 };
			uint8_t dt = ses->nfscreenType;
			memcpy( &d, ses->nfscreen, sizeof( sbWnd_Dims ) );
			Commands.Download.Ses.End( ses );
			if ( ses->fscreenOn )
				SBWindows.changeDims( p, dt, &d );
			break;
		}

	}
	return;
}