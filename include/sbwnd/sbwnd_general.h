#ifndef SBWND_GENERAL_MODULE
#define SBWND_GENERAL_MODULE

#include <stdint.h>
#include <gdiplus.h>

#include "../sb2/sb_ds.h"

/* This header file describes general purpose window structs for sbgui.
 * It has the sbWnd struct, specific window information structs, and the window function struct.
 */ 

#define BASIC_WINDOW			0
#define TEXTBOX_WINDOW			1
#define TEXT_WINDOW				2
#define CLICKABLE_WINDOW		3
#define RESTRICTED_IMAGE_WINDOW	4
#define MASTER_WINDOW			5
#define VIEWCMD_MASTER_WINDOW	6
#define STRING_WINDOW			7
#define PROGRESS_BAR_WINDOW		8
#define DLDCMD_MASTER_WINDOW	9
#define VSCROLLBAR_WINDOW		10

#define BASICWND
#define TBOX
#define BASICTXTWND
#define CLICKWND
#define RSTIMGWND
#define MSTWND
#define VIEWMSTWND
#define STRWND
#define PROGBARWND
#define DLDMSTWND
#define VSCROLLWND

#define GENERICWND

#define SB_CENTER_DIM(dim)	(-(dim))

// Current number of total window classes availible to sbgui.
#define NUM_CLASSES     		(11)

// Message that tells the window to reset its font to what should be a different font size, according to the size of the window. The LPARAM will contain a boolean that, if true, tells the window to size the font to the window's full height, and if false, the window will use WPARAM as the font size. Initialized on initGUI().
#define SBM_CHANGEFONTSIZE  (WM_APP+0x01)

// Message that tells a text window to redraw its text according to a new font size.
#define SBM_REDRAWTEXT      (WM_APP+0x02)

// WM_COMMAND but for sbgui windows. WPARAM is not used, and LPARAM contains the window's id.
#define SBM_COMMAND			(WM_APP+0x03)

// Tells all windows to change the current font (if fonts are applicable) and rerender their text.
#define SBM_CHANGEFONT		(WM_APP+0x04)

typedef int		dimension;
typedef char *	String;

typedef union dimuinion {
	int32_t intDims[4];
	float floatDims[4];
} sbWnd_Dims;

typedef struct sbloutstruct {

	// Union of integers and floats (See above typedef). Each of the four array
	// slots can only be an integer or a float. Floats will
	// be evaluated against the parent rectangle. Ints are
	// relative to the specified corners in the dimType member.
	sbWnd_Dims dims;

	// SB_DIMTYPE_XXXX_XX_XXXX
	// The first four Xs are either F or I, for float or int.
	// The second two Xs specify the coordinate space: TL, TR, BL, or BR.
	// It refers to the corner of the parent window and the corner of the
	// window to be created--both corners are specified at once.
	// The last 4 Xs specify the sibling-relative coordinate space.
	// They can either be SRTL, SRTR, SRBL, SRBR, or SRNN for a placeholder
	// "nothing". If a sibling is given, the last 4 Xs will be the sibling
	// corner, and the middle 2 Xs will be the window corner.
	// For example, a window that takes up the top left 25 percent of
	// its parent will have a type of SB_DIMTYPE_IIFF_TL_SRNN.
	// A constant width and heightright side bar would have a type
	// of SB_DIMTYPE_IIII_TR_SRNN. A constant size window whose top 
	// right corner is placed next to the top left corner of that window would
	// have a type of SB_DIMTYPE_IIII_TR_SRTL.
	uint8_t type;

	struct _Saucebot_Window_2
		*parent,
		*sibling;

	dimension offset[2];

} sbWnd_Layout;


typedef wchar_t *	wString;
typedef wchar_t		wChar;

#define SBWINDOW_TOP	0
#define SBWINDOW_LEFT	1
#define	SBWINDOW_HEIGHT	2
#define SBWINDOW_WIDTH	3

#define SBWINDOW_BOTTOM	2
#define SBWINDOW_RIGHT	3

typedef struct _Saucebot_Window_2 {

	// Windows handle to window. The sbWnd type encapsulates this.
	HWND
		hwnd,
	// Parent HWND, if applicable.
		parent;

	struct _Saucebot_Window_2
		*parentSb,
		*sibling;

	// Window ID used as an identifier for sbgui window messages.
	// Why is this here, again?
	uint16_t id;

	// The type of the window. It is used to validate the use of functions from the SBWindow function struct.
	uint16_t type;

	wString name;

	sbWnd_Layout *lout;
	
	// Specific data that pertains to the window according to its type.
	void *specific;

	// A way to keep other windows on hand easily.
	sbList *referenceList;

	// Function that accepts data, specific to a window type. Can be user-overridden, however.
	void ( *signalFn )( struct _Saucebot_Window_2 *, void * );

} sbWnd;

typedef void ( *sbWndSignalFn )( struct _Saucebot_Window_2 *, void * );

typedef struct tboxinfo {

	// Current position of the caret based on the window rectangle.
	int currentCaretPosition;

	// Tells if the textbox can display another character or not.
	int canDisplay;

	// The String the textbox puts its characters into.
	wString tiedown;

	// Current length of the tiedown string.
	size_t tiedownLength;

	// Current capacity of the tiedown buffer.
	size_t tiedownCapacity;

	// Rectangle that represents the textbox. This is smaller than the window rectangle and fits inside the drawn borders of the textbox.
	RECT *tbRect;

	// Current window font. Activated whenever a WM_CHAR message is sent.
	LOGFONTW *currentFont;

	// Action when enter is pressed. 0 for out function, 1 for nothing--not even clearing the string.
	int enterAction;

} SBTextboxInfo;


typedef struct basictextinfo {

	// A list that holds lines of text. It is seperated into lines, each node of the list contains one line of text.
	sbList *text;

	sbWnd *scrollbar;

	uint16_t
		currentOffset,
		currentMaxLines,
		prevMaxLines,
		lineCount,
		fontSize;

	LOGFONTW *currentFont;

} SBBasicTextWindowInfo;


typedef struct clickableinfo {

	// Current window font.
	GpFontFamily *currentFont;

} SBClickableWindowInfo;


typedef struct imagewndinfo {

	// Path of the image drawn onto the window.
	wString path;

	// Current image to be drawn.
	GpImage *img;

	uint32_t
	// Maximum width (in px) of the image.
		maxWidth,
	// Maximum height (in px) of the image.
		maxHeight;

	// Aspect ratio of the image (width / height).
	double aspectRatio;

} SBRestrictedImageWindowInfo;

typedef struct strwndinfo {

	wString str;

	uint16_t fontSize;

	LOGFONTW *currentFont;

	int 
		clickable, // denotes if window is clickable
		mouseState; // Bit 0 is for hover, Bit 1 is for button down

} SBStringWindowInfo;

typedef struct progbarwndinfo {

	uint16_t
		total,
		cur;

} SBProgressBarWindowInfo;

typedef struct vscrollwndinfo {

	uint16_t
		incDist,
		maxInc,
		cur;

} SBVScrollbarWindowInfo;

typedef struct {

	struct {

		// This is a test.
		// Syntax of creators soon to be:
		sbWnd *( *create )( const wString, sbWnd_Layout * );

	} SBBasicWindow;


	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout *, int );

		void
			( *sendString )( sbWnd *, wString ),
			( *setEnterAction )( sbWnd *, int );

		wString ( *getString )( sbWnd * );

		int ( *getEnterAction )( sbWnd * );

	} SBTextbox;


	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout *, uint16_t );

		void
			( *draw )( sbWnd *, wString ),
			( *clear )( sbWnd * );

	} SBBasicTextWindow;


	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout * );
		
	} SBClickableWindow;


	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout *, const wString );

		void ( *updateImage )( sbWnd *, const wString );

	} SBRestrictedImageWindow;

	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout * );

	} SBMasterWindow;

	struct {

		sbWnd *( *create )( const wString, sbWnd_Layout * );

	} SBViewcmdMasterWindow;

	struct {

		// Set last parameter to nonzero for clickable, zero for static.
		sbWnd *( *create )( const wString, sbWnd_Layout *, const wString, uint16_t, int );

		void ( *changeString )( sbWnd *, const wString );

	} SBStringWindow;

	struct {

		PROGBARWND sbWnd *( *create )( const wString, sbWnd_Layout *, uint16_t );

		void ( *advance )( PROGBARWND sbWnd * );

	} SBProgressBarWindow;

	struct {

		DLDMSTWND sbWnd *( *create )( const wString, sbWnd_Layout * );

	} SBDldcmdMasterWindow;

	struct {

		VSCROLLWND sbWnd *( *create )( const wString, sbWnd_Layout *, uint16_t );

		void
			( *advance )( VSCROLLWND sbWnd * ),
			( *retreat )( VSCROLLWND sbWnd * ),
			( *reset )( VSCROLLWND sbWnd * ),
			( *setPos )( VSCROLLWND sbWnd *, uint16_t ),
			( *setMaxIncrement )( VSCROLLWND sbWnd *, uint16_t );

		int
			( *getMaxIncrement )( VSCROLLWND sbWnd * ),
			( *getCurrentPos )( VSCROLLWND sbWnd * );

	} SBVScrollbarWindow;

	void ( *destroy )( sbWnd * );

	// Appends data to the window's reference list. The user is responsible for keeping track of what data is at which index.
	void ( *appendReference )( sbWnd *, void * );

	// Gets some refernece data from the window's reference list. Zero-indexed.
	void *( *getReference )( sbWnd *, uint16_t );

	// This function computes the width of the given string using the global font face and the given height.
	uint32_t ( *getStringWidth )( wString, uint32_t );

	uint16_t ( *getID )( sbWnd * );

	uint16_t ( *getType )( sbWnd * );
	
	void ( *setCreateMode )( uint8_t );

	// Shows a window. The second parameter should be a nonzero value
	// unless you are showing a window because of resizing, in which
	// case it should be 0.
	void ( *show )( sbWnd *, int );

	// Hides a window. The second parameter should be a nonzero value
	// unless you are hiding a window because of resizing, in which
	// case it should be 0.
	void ( *hide )( sbWnd *, int );

	void ( *update )( GENERICWND sbWnd * );

	void ( *getLayout )( sbWnd *, sbWnd_Layout * );

	void ( *addOffset )( sbWnd *, dimension, dimension );

	sbWnd *( *getParent )( sbWnd * );

	void ( *changeLayout )( sbWnd *, sbWnd_Layout * );

	void ( *toSurface )( sbWnd * );
	void ( *focus )( sbWnd * );


	void ( *signalFn )( sbWnd *, sbWnd *, void * );

	void ( *setSignalFn )( sbWnd *, sbWndSignalFn );

} SBWindowFnStruct;

extern SBWindowFnStruct SBWindow;

#define SBWindows					SBWindow
#define SBBasicWindows				SBWindow.SBBasicWindow
#define SBTextboxes					SBWindow.SBTextbox
#define SBBasicTextWindows			SBWindow.SBBasicTextWindow
#define SBClickableWindows			SBWindow.SBClickableWindow
#define SBRestrictedImageWindows	SBWindow.SBRestrictedImageWindow
#define SBMasterWindows				SBWindow.SBMasterWindow
#define SBViewcmdMasterWindows		SBWindow.SBViewcmdMasterWindow
#define SBStringWindows				SBWindow.SBStringWindow
#define SBProgressBarWindows		SBWindow.SBProgressBarWindow
#define SBDldcmdMasterWindows		SBWindow.SBDldcmdMasterWindow
#define SBVScrollbarWindows			SBWindow.SBVScrollbarWindow


HWND GetHWND( GENERICWND sbWnd * );

void *GetSpecificHandle( GENERICWND sbWnd * );

void msgloop( void );

GENERICWND

void SetSignalFn_AllTypes( GENERICWND sbWnd *, sbWndSignalFn );
void CallSignalFn_AllTypes( GENERICWND sbWnd *, GENERICWND sbWnd *, void * );

void appendReference_AllTypes( GENERICWND sbWnd *, void * );
void *getReference_AllTypes( GENERICWND sbWnd *, uint16_t );

uint32_t getStringWidth_AllTypes( wString, uint32_t );

uint16_t getID_AllTypes( GENERICWND sbWnd * );
uint16_t getType_AllTypes( GENERICWND sbWnd * );

void setCreateMode_AllTypes( uint8_t );
void show_AllTypes( GENERICWND sbWnd *, int );
void hide_AllTypes( GENERICWND sbWnd *, int );
void update_AllTypes( GENERICWND sbWnd * );
void changeLayout_AllTypes( GENERICWND sbWnd *, sbWnd_Layout * );
void addOffset_AllTypes( GENERICWND sbWnd *, dimension, dimension );
void toSurface_AllTypes( GENERICWND sbWnd * );
void focus_AllTypes( GENERICWND sbWnd * );
void getLayout_AllTypes( GENERICWND sbWnd *, sbWnd_Layout * );
sbWnd *getParent_AllTypes( GENERICWND sbWnd * );

TBOX

wString getString_SbTextbox( TBOX sbWnd * );
void sendString_SbTextbox( TBOX sbWnd *, wString );
void setEnterAction_SbTextbox( TBOX sbWnd *, int );
int getEnterAction_SbTextbox( TBOX sbWnd * );

BASICTXTWND

void draw_SbBasicTextWindow( BASICTXTWND sbWnd *, wString );
void clear_SbBasicTextWindow( BASICTXTWND sbWnd * );

RSTIMGWND

void updateImage_SbRestrictedImageWindow( RSTIMGWND sbWnd *, const wString );

STRWND

void changeString_SbStringWindow( STRWND sbWnd *, const wString );

PROGBARWND

void advance_SbProgressBarWindow( PROGBARWND sbWnd * );

VSCROLLWND

void
	advance_SbVScrollbarWindow( VSCROLLWND sbWnd * ),
	retreat_SbVScrollbarWindow( VSCROLLWND sbWnd * ),
	reset_SbVScrollbarWindow( VSCROLLWND sbWnd * ),
	setPos_SbVScrollbarWindow( VSCROLLWND sbWnd *, uint16_t ),
	setMaxIncrement_SbVScrollbarWindow( VSCROLLWND sbWnd *, uint16_t );

int
	getMaxIncrement_SbVScrollbarWindow( VSCROLLWND sbWnd * ),
	getCurrentPos_SbVScrollbarWindow( VSCROLLWND sbWnd * );

#endif