#ifndef SBWND_CREATE_MODULE
#define SBWND_CREATE_MODULE

#include <windows.h>
#include "sbwnd_general.h"

sbWnd *CommonInfoCreator( HWND, const wString, uint8_t, sbWnd_Dims * );


#define SB_DIMTYPE_IIII_TL		(0x00)
#define SB_DIMTYPE_IIIF_TL		(0x01)
#define SB_DIMTYPE_IIFI_TL		(0x02)
#define SB_DIMTYPE_IIFF_TL		(0x03)
#define SB_DIMTYPE_IFII_TL		(0x04)
#define SB_DIMTYPE_IFIF_TL		(0x05)
#define SB_DIMTYPE_IFFI_TL		(0x06)
#define SB_DIMTYPE_IFFF_TL		(0x07)
#define SB_DIMTYPE_FIII_TL		(0x08)
#define SB_DIMTYPE_FIIF_TL		(0x09)
#define SB_DIMTYPE_FIFI_TL		(0x0A)
#define SB_DIMTYPE_FIFF_TL		(0x0B)
#define SB_DIMTYPE_FFII_TL		(0x0C)
#define SB_DIMTYPE_FFIF_TL		(0x0D)
#define SB_DIMTYPE_FFFI_TL		(0x0E)
#define SB_DIMTYPE_FFFF_TL		(0x0F)
#define SB_DIMTYPE_IIII_TR		(0x10)
#define SB_DIMTYPE_IIIF_TR		(0x11)
#define SB_DIMTYPE_IIFI_TR		(0x12)
#define SB_DIMTYPE_IIFF_TR		(0x13)
#define SB_DIMTYPE_IFII_TR		(0x14)
#define SB_DIMTYPE_IFIF_TR		(0x15)
#define SB_DIMTYPE_IFFI_TR		(0x16)
#define SB_DIMTYPE_IFFF_TR		(0x17)
#define SB_DIMTYPE_FIII_TR		(0x18)
#define SB_DIMTYPE_FIIF_TR		(0x19)
#define SB_DIMTYPE_FIFI_TR		(0x1A)
#define SB_DIMTYPE_FIFF_TR		(0x1B)
#define SB_DIMTYPE_FFII_TR		(0x1C)
#define SB_DIMTYPE_FFIF_TR		(0x1D)
#define SB_DIMTYPE_FFFI_TR		(0x1E)
#define SB_DIMTYPE_FFFF_TR		(0x1F)
#define SB_DIMTYPE_IIII_BL		(0x20)
#define SB_DIMTYPE_IIIF_BL		(0x21)
#define SB_DIMTYPE_IIFI_BL		(0x22)
#define SB_DIMTYPE_IIFF_BL		(0x23)
#define SB_DIMTYPE_IFII_BL		(0x24)
#define SB_DIMTYPE_IFIF_BL		(0x25)
#define SB_DIMTYPE_IFFI_BL		(0x26)
#define SB_DIMTYPE_IFFF_BL		(0x27)
#define SB_DIMTYPE_FIII_BL		(0x28)
#define SB_DIMTYPE_FIIF_BL		(0x29)
#define SB_DIMTYPE_FIFI_BL		(0x2A)
#define SB_DIMTYPE_FIFF_BL		(0x2B)
#define SB_DIMTYPE_FFII_BL		(0x2C)
#define SB_DIMTYPE_FFIF_BL		(0x2D)
#define SB_DIMTYPE_FFFI_BL		(0x2E)
#define SB_DIMTYPE_FFFF_BL		(0x2F)
#define SB_DIMTYPE_IIII_BR		(0x30)
#define SB_DIMTYPE_IIIF_BR		(0x31)
#define SB_DIMTYPE_IIFI_BR		(0x32)
#define SB_DIMTYPE_IIFF_BR		(0x33)
#define SB_DIMTYPE_IFII_BR		(0x34)
#define SB_DIMTYPE_IFIF_BR		(0x35)
#define SB_DIMTYPE_IFFI_BR		(0x36)
#define SB_DIMTYPE_IFFF_BR		(0x37)
#define SB_DIMTYPE_FIII_BR		(0x38)
#define SB_DIMTYPE_FIIF_BR		(0x39)
#define SB_DIMTYPE_FIFI_BR		(0x3A)
#define SB_DIMTYPE_FIFF_BR		(0x3B)
#define SB_DIMTYPE_FFII_BR		(0x3C)
#define SB_DIMTYPE_FFIF_BR		(0x3D)
#define SB_DIMTYPE_FFFI_BR		(0x3E)
#define SB_DIMTYPE_FFFF_BR		(0x3F)


HWND BasicWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND TextboxHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND BasicTextWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND ClickableWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND RestrictedImageWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND MasterWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND ViewcmdMasterWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND StringWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND ProgressBarWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
HWND DldcmdMasterWindowHWNDCreator( HWND, const wString, uint8_t, sbWnd_Dims * );


sbWnd *BasicWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
sbWnd *TextboxCreator( HWND, const wString, uint8_t, sbWnd_Dims *, int );
sbWnd *BasicTextWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims *, uint16_t );
sbWnd *ClickableWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
sbWnd *RestrictedImageWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims *, const wString );
sbWnd *MasterWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
sbWnd *ViewcmdMasterWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims * );
sbWnd *StringWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims *, const wString, uint16_t );
sbWnd *ProgressBarWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims *, uint16_t );
sbWnd *DldcmdMasterWindowCreator( HWND, const wString, uint8_t, sbWnd_Dims * );


void destroy_SbBasicWindow( sbWnd * );
void destroy_SbTextbox( sbWnd * );
void destroy_SbBasicTextWindow( sbWnd * );
void destroy_SbClickableWindow( sbWnd * );
void destroy_SbRestrictedImageWindow( sbWnd * );
void destroy_SbMasterWindow( sbWnd * );
void destroy_SbViewcmdMasterWindow( sbWnd * );
void destroy_SbStringWindow( sbWnd * );
void destroy_SbProgressBarWindow( sbWnd * );
void destroy_SbDldcmdMasterWindow( sbWnd * );

void sbWnd_DeleteTextboxInfo( SBTextboxInfo * );
void sbWnd_DeleteBasicTextWindowInfo( SBBasicTextWindowInfo * );
void sbWnd_DeleteClickableWindowInfo( SBClickableWindowInfo * );
void sbWnd_DeleteRestrictedImageWindowInfo( SBRestrictedImageWindowInfo * );
void sbWnd_DeleteStringWindowInfo( SBStringWindowInfo * );
void sbWnd_DeleteProgressBarWindowInfo( SBProgressBarWindowInfo * );

SBTextboxInfo *TextboxInfoCreator( HWND, int );
SBBasicTextWindowInfo *BasicTextWindowInfoCreator( HWND, uint16_t );
SBClickableWindowInfo *ClickableWindowInfoCreator( void );
SBRestrictedImageWindowInfo *RestrictedImageWindowInfoCreator( HWND, const wString );
SBStringWindowInfo *StringWindowInfoCreator( const wString, uint16_t );
SBProgressBarWindowInfo *ProgressBarWindowInfoCreator( uint16_t );

void sbWnd_ProcessQueue( HWND );

#endif