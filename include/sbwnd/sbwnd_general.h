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

#define GENERICWND

#define SB_CENTER_DIM(dim)	(-(dim))

// Current number of total window classes availible to sbgui.
#define NUM_CLASSES     		(10)

// Message that tells the window to reset its font to what should be a different font size, according to the size of the window. The LPARAM will contain a boolean that, if true, tells the window to size the font to the window's full height, and if false, the window will use WPARAM as the font size. Initialized on initGUI().
#define SBM_CHANGEFONTSIZE  (WM_APP+0x01)

// Message that tells a text window to redraw its text according to a new font size.
#define SBM_REDRAWTEXT      (WM_APP+0x02)

// WM_COMMAND but for sbgui windows. WPARAM is not used, and LPARAM contains the window's id.
#define SBM_COMMAND			(WM_APP+0x03)

// Tells all windows to change the current font (if fonts are applicable) and rerender their text.
#define SBM_CHANGEFONT		(WM_APP+0x04)

typedef int			dimension;
typedef double		scalingCoefficient;
typedef char *		String;

typedef union dimuinion {
	int32_t intDims[4];
	float floatDims[4];
} sbWnd_Dims;

/*
typedef struct sbdimstruct {

	union {
		int32_t intDims[4];
		float floatDims[4];
	} dims;

	uint8_t dimType;

	sbWnd
		*parent,
		*sibling;

} sbWnd_Dims;
*/

typedef wchar_t *	wString;
typedef wchar_t		wChar;

#define SBWINDOW_TOP	0
#define SBWINDOW_LEFT	1
#define	SBWINDOW_HEIGHT	2
#define SBWINDOW_WIDTH	3

#define SBWINDOW_BOTTOM	2
#define SBWINDOW_RIGHT	3

typedef struct _Saucebot_Window_2 {

	// Windows type that represents the window. Also the key for the window in the master window map.
	HWND hwnd;

	// Parent HWND, if applicable.
	HWND parent;

	// Window ID used as an identifier for sbgui window messages.
	uint16_t id;

	// The type of the window. It is used to validate the use of functions from the SBWindow function struct.
	uint16_t type;

	// The name of the window, used in the Windows API.
	wString name;

	// Type of dimensions used on the window. 0 for integer dimensions, nonzero for scaling coefficients.
	uint8_t dimType;

	// Pointer to the dimensions used.
	sbWnd_Dims *dims;
	
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

	// Current line to draw to of the text window.
	uint16_t currentLine;
	
	// Font size of the window.
	uint16_t fontSize;

	// Current window font.
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

	// Maximum width (in px) of the image.
	uint32_t maxWidth;

	// Maximum height (in px) of the image.
	uint32_t maxHeight;

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

	uint16_t total;

	uint16_t cur;

} SBProgressBarWindowInfo;

typedef struct {

	struct {

		// Syntax of creators soon to be:
		// sbWnd *( *create )( const wString name, sbWnd_Dims* dims );
		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims * );

		void ( *destroy )( sbWnd * );

	} SBBasicWindow;


	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims *, int );

		void ( *destroy )( sbWnd * );

		void ( *sendString )( sbWnd *, wString );

		wString ( *getString )( sbWnd * );

		void ( *setEnterAction )( sbWnd *, int );

		int ( *getEnterAction )( sbWnd * );

	} SBTextbox;


	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims *, uint16_t );

		void ( *destroy )( sbWnd * );

		void ( *draw )( sbWnd *, wString );

		void ( *clear )( sbWnd * );

	} SBBasicTextWindow;


	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims * );

		void ( *destroy )( sbWnd * );
		
	} SBClickableWindow;


	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims *, const wString );

		void ( *destroy )( sbWnd * );

		void ( *updateImage )( sbWnd *, const wString );

	} SBRestrictedImageWindow;

	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims * );

		void ( *destroy )( sbWnd * );

	} SBMasterWindow;

	struct {

		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims * );

		void ( *destroy )( sbWnd * );

	} SBViewcmdMasterWindow;

	struct {

		// Set last parameter to nonzero for clickable, zero for static.
		sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims *, const wString, uint16_t, int );

		void ( *destroy )( sbWnd * );

		void ( *changeString )( sbWnd *, const wString );

	} SBStringWindow;

	struct {

		PROGBARWND sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims *, uint16_t );

		void ( *destroy )( PROGBARWND sbWnd * );

		void ( *advance )( PROGBARWND sbWnd * );

	} SBProgressBarWindow;

	struct {

		DLDMSTWND sbWnd *( *create )( HWND, const wString, uint8_t, sbWnd_Dims * );

		void ( *destroy )( DLDMSTWND sbWnd * );

	} SBDldcmdMasterWindow;

	// Appends data to the window's reference list. The user is responsible for keeping track of what data is at which index.
	void ( *appendReference )( sbWnd *, void * );

	// Gets some refernece data from the window's reference list. Zero-indexed.
	void *( *getReference )( sbWnd *, uint16_t );

	// This function computes the width of the given string using the global font face and the given height.
	uint32_t ( *getStringWidth )( wString, uint32_t );

	uint16_t ( *getID )( sbWnd * );

	uint16_t ( *getType )( sbWnd * );
	
	void ( *setCreateMode )( uint8_t );

	void ( *show )( sbWnd * );

	void ( *hide )( sbWnd * );	

	void ( *getDims )( sbWnd *, uint8_t *, sbWnd_Dims * );

	sbWnd *( *getParent )( sbWnd * );

	// Copies new dimensions in and updates window.
	void ( *changeDims )( sbWnd *, uint8_t, sbWnd_Dims * );

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


HWND GetHWND( GENERICWND sbWnd * );

void *GetSpecificHandle( GENERICWND sbWnd * );

void msgloop( void );

void SetSignalFn_AllTypes( GENERICWND sbWnd *, sbWndSignalFn );
void CallSignalFn_AllTypes( GENERICWND sbWnd *, GENERICWND sbWnd *, void * );

void appendReference_AllTypes( GENERICWND sbWnd *, void * );
void *getReference_AllTypes( GENERICWND sbWnd *, uint16_t );

uint32_t getStringWidth_AllTypes( wString, uint32_t );

uint16_t getID_AllTypes( GENERICWND sbWnd * );
uint16_t getType_AllTypes( GENERICWND sbWnd * );

void setCreateMode_AllTypes( uint8_t );
void show_AllTypes( GENERICWND sbWnd * );
void hide_AllTypes( GENERICWND sbWnd * );
void changeDims_AllTypes( GENERICWND sbWnd *, uint8_t, sbWnd_Dims * );
void toSurface_AllTypes( GENERICWND sbWnd * );
void focus_AllTypes( GENERICWND sbWnd * );
void getDims_AllTypes( GENERICWND sbWnd *, uint8_t *, sbWnd_Dims * );
sbWnd *getParent_AllTypes( GENERICWND sbWnd * );

wString getString_SbTextbox( TBOX sbWnd * );
void sendString_SbTextbox( TBOX sbWnd *, wString );
void setEnterAction_SbTextbox( TBOX sbWnd *, int );
int getEnterAction_SbTextbox( TBOX sbWnd * );

void draw_SbBasicTextWindow( BASICTXTWND sbWnd *, wString );
void clear_SbBasicTextWindow( BASICTXTWND sbWnd * );

void updateImage_SbRestrictedImageWindow( RSTIMGWND sbWnd *, const wString );

void changeString_SbStringWindow( STRWND sbWnd *, const wString );

void advance_SbProgressBarWindow( PROGBARWND sbWnd * );

#endif