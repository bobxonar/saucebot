#ifndef SBWND_CREATE_MODULE
#define SBWND_CREATE_MODULE

#include <windows.h>
#include "sbwnd_general.h"

sbWnd *CommonInfoCreatorLayout( const wString, sbWnd_Layout * );

#define SB_DIMTYPE_IIII_TL_SRNN		(0x00)
#define SB_DIMTYPE_IIIF_TL_SRNN		(0x01)
#define SB_DIMTYPE_IIFI_TL_SRNN		(0x02)
#define SB_DIMTYPE_IIFF_TL_SRNN		(0x03)
#define SB_DIMTYPE_IFII_TL_SRNN		(0x04)
#define SB_DIMTYPE_IFIF_TL_SRNN		(0x05)
#define SB_DIMTYPE_IFFI_TL_SRNN		(0x06)
#define SB_DIMTYPE_IFFF_TL_SRNN		(0x07)
#define SB_DIMTYPE_FIII_TL_SRNN		(0x08)
#define SB_DIMTYPE_FIIF_TL_SRNN		(0x09)
#define SB_DIMTYPE_FIFI_TL_SRNN		(0x0A)
#define SB_DIMTYPE_FIFF_TL_SRNN		(0x0B)
#define SB_DIMTYPE_FFII_TL_SRNN		(0x0C)
#define SB_DIMTYPE_FFIF_TL_SRNN		(0x0D)
#define SB_DIMTYPE_FFFI_TL_SRNN		(0x0E)
#define SB_DIMTYPE_FFFF_TL_SRNN		(0x0F)
#define SB_DIMTYPE_IIII_TR_SRNN		(0x10)
#define SB_DIMTYPE_IIIF_TR_SRNN		(0x11)
#define SB_DIMTYPE_IIFI_TR_SRNN		(0x12)
#define SB_DIMTYPE_IIFF_TR_SRNN		(0x13)
#define SB_DIMTYPE_IFII_TR_SRNN		(0x14)
#define SB_DIMTYPE_IFIF_TR_SRNN		(0x15)
#define SB_DIMTYPE_IFFI_TR_SRNN		(0x16)
#define SB_DIMTYPE_IFFF_TR_SRNN		(0x17)
#define SB_DIMTYPE_FIII_TR_SRNN		(0x18)
#define SB_DIMTYPE_FIIF_TR_SRNN		(0x19)
#define SB_DIMTYPE_FIFI_TR_SRNN		(0x1A)
#define SB_DIMTYPE_FIFF_TR_SRNN		(0x1B)
#define SB_DIMTYPE_FFII_TR_SRNN		(0x1C)
#define SB_DIMTYPE_FFIF_TR_SRNN		(0x1D)
#define SB_DIMTYPE_FFFI_TR_SRNN		(0x1E)
#define SB_DIMTYPE_FFFF_TR_SRNN		(0x1F)
#define SB_DIMTYPE_IIII_BL_SRNN		(0x20)
#define SB_DIMTYPE_IIIF_BL_SRNN		(0x21)
#define SB_DIMTYPE_IIFI_BL_SRNN		(0x22)
#define SB_DIMTYPE_IIFF_BL_SRNN		(0x23)
#define SB_DIMTYPE_IFII_BL_SRNN		(0x24)
#define SB_DIMTYPE_IFIF_BL_SRNN		(0x25)
#define SB_DIMTYPE_IFFI_BL_SRNN		(0x26)
#define SB_DIMTYPE_IFFF_BL_SRNN		(0x27)
#define SB_DIMTYPE_FIII_BL_SRNN		(0x28)
#define SB_DIMTYPE_FIIF_BL_SRNN		(0x29)
#define SB_DIMTYPE_FIFI_BL_SRNN		(0x2A)
#define SB_DIMTYPE_FIFF_BL_SRNN		(0x2B)
#define SB_DIMTYPE_FFII_BL_SRNN		(0x2C)
#define SB_DIMTYPE_FFIF_BL_SRNN		(0x2D)
#define SB_DIMTYPE_FFFI_BL_SRNN		(0x2E)
#define SB_DIMTYPE_FFFF_BL_SRNN		(0x2F)
#define SB_DIMTYPE_IIII_BR_SRNN		(0x30)
#define SB_DIMTYPE_IIIF_BR_SRNN		(0x31)
#define SB_DIMTYPE_IIFI_BR_SRNN		(0x32)
#define SB_DIMTYPE_IIFF_BR_SRNN		(0x33)
#define SB_DIMTYPE_IFII_BR_SRNN		(0x34)
#define SB_DIMTYPE_IFIF_BR_SRNN		(0x35)
#define SB_DIMTYPE_IFFI_BR_SRNN		(0x36)
#define SB_DIMTYPE_IFFF_BR_SRNN		(0x37)
#define SB_DIMTYPE_FIII_BR_SRNN		(0x38)
#define SB_DIMTYPE_FIIF_BR_SRNN		(0x39)
#define SB_DIMTYPE_FIFI_BR_SRNN		(0x3A)
#define SB_DIMTYPE_FIFF_BR_SRNN		(0x3B)
#define SB_DIMTYPE_FFII_BR_SRNN		(0x3C)
#define SB_DIMTYPE_FFIF_BR_SRNN		(0x3D)
#define SB_DIMTYPE_FFFI_BR_SRNN		(0x3E)
#define SB_DIMTYPE_FFFF_BR_SRNN		(0x3F)


#define SB_DIMTYPE_IIII_TL_SRTL		(0x00)
#define SB_DIMTYPE_IIIF_TL_SRTL		(0x01)
#define SB_DIMTYPE_IIFI_TL_SRTL		(0x02)
#define SB_DIMTYPE_IIFF_TL_SRTL		(0x03)
#define SB_DIMTYPE_IFII_TL_SRTL		(0x04)
#define SB_DIMTYPE_IFIF_TL_SRTL		(0x05)
#define SB_DIMTYPE_IFFI_TL_SRTL		(0x06)
#define SB_DIMTYPE_IFFF_TL_SRTL		(0x07)
#define SB_DIMTYPE_FIII_TL_SRTL		(0x08)
#define SB_DIMTYPE_FIIF_TL_SRTL		(0x09)
#define SB_DIMTYPE_FIFI_TL_SRTL		(0x0A)
#define SB_DIMTYPE_FIFF_TL_SRTL		(0x0B)
#define SB_DIMTYPE_FFII_TL_SRTL		(0x0C)
#define SB_DIMTYPE_FFIF_TL_SRTL		(0x0D)
#define SB_DIMTYPE_FFFI_TL_SRTL		(0x0E)
#define SB_DIMTYPE_FFFF_TL_SRTL		(0x0F)
#define SB_DIMTYPE_IIII_TR_SRTL		(0x10)
#define SB_DIMTYPE_IIIF_TR_SRTL		(0x11)
#define SB_DIMTYPE_IIFI_TR_SRTL		(0x12)
#define SB_DIMTYPE_IIFF_TR_SRTL		(0x13)
#define SB_DIMTYPE_IFII_TR_SRTL		(0x14)
#define SB_DIMTYPE_IFIF_TR_SRTL		(0x15)
#define SB_DIMTYPE_IFFI_TR_SRTL		(0x16)
#define SB_DIMTYPE_IFFF_TR_SRTL		(0x17)
#define SB_DIMTYPE_FIII_TR_SRTL		(0x18)
#define SB_DIMTYPE_FIIF_TR_SRTL		(0x19)
#define SB_DIMTYPE_FIFI_TR_SRTL		(0x1A)
#define SB_DIMTYPE_FIFF_TR_SRTL		(0x1B)
#define SB_DIMTYPE_FFII_TR_SRTL		(0x1C)
#define SB_DIMTYPE_FFIF_TR_SRTL		(0x1D)
#define SB_DIMTYPE_FFFI_TR_SRTL		(0x1E)
#define SB_DIMTYPE_FFFF_TR_SRTL		(0x1F)
#define SB_DIMTYPE_IIII_BL_SRTL		(0x20)
#define SB_DIMTYPE_IIIF_BL_SRTL		(0x21)
#define SB_DIMTYPE_IIFI_BL_SRTL		(0x22)
#define SB_DIMTYPE_IIFF_BL_SRTL		(0x23)
#define SB_DIMTYPE_IFII_BL_SRTL		(0x24)
#define SB_DIMTYPE_IFIF_BL_SRTL		(0x25)
#define SB_DIMTYPE_IFFI_BL_SRTL		(0x26)
#define SB_DIMTYPE_IFFF_BL_SRTL		(0x27)
#define SB_DIMTYPE_FIII_BL_SRTL		(0x28)
#define SB_DIMTYPE_FIIF_BL_SRTL		(0x29)
#define SB_DIMTYPE_FIFI_BL_SRTL		(0x2A)
#define SB_DIMTYPE_FIFF_BL_SRTL		(0x2B)
#define SB_DIMTYPE_FFII_BL_SRTL		(0x2C)
#define SB_DIMTYPE_FFIF_BL_SRTL		(0x2D)
#define SB_DIMTYPE_FFFI_BL_SRTL		(0x2E)
#define SB_DIMTYPE_FFFF_BL_SRTL		(0x2F)
#define SB_DIMTYPE_IIII_BR_SRTL		(0x30)
#define SB_DIMTYPE_IIIF_BR_SRTL		(0x31)
#define SB_DIMTYPE_IIFI_BR_SRTL		(0x32)
#define SB_DIMTYPE_IIFF_BR_SRTL		(0x33)
#define SB_DIMTYPE_IFII_BR_SRTL		(0x34)
#define SB_DIMTYPE_IFIF_BR_SRTL		(0x35)
#define SB_DIMTYPE_IFFI_BR_SRTL		(0x36)
#define SB_DIMTYPE_IFFF_BR_SRTL		(0x37)
#define SB_DIMTYPE_FIII_BR_SRTL		(0x38)
#define SB_DIMTYPE_FIIF_BR_SRTL		(0x39)
#define SB_DIMTYPE_FIFI_BR_SRTL		(0x3A)
#define SB_DIMTYPE_FIFF_BR_SRTL		(0x3B)
#define SB_DIMTYPE_FFII_BR_SRTL		(0x3C)
#define SB_DIMTYPE_FFIF_BR_SRTL		(0x3D)
#define SB_DIMTYPE_FFFI_BR_SRTL		(0x3E)
#define SB_DIMTYPE_FFFF_BR_SRTL		(0x3F)


#define SB_DIMTYPE_IIII_TL_SRTR		(0x00|0x40)
#define SB_DIMTYPE_IIIF_TL_SRTR		(0x01|0x40)
#define SB_DIMTYPE_IIFI_TL_SRTR		(0x02|0x40)
#define SB_DIMTYPE_IIFF_TL_SRTR		(0x03|0x40)
#define SB_DIMTYPE_IFII_TL_SRTR		(0x04|0x40)
#define SB_DIMTYPE_IFIF_TL_SRTR		(0x05|0x40)
#define SB_DIMTYPE_IFFI_TL_SRTR		(0x06|0x40)
#define SB_DIMTYPE_IFFF_TL_SRTR		(0x07|0x40)
#define SB_DIMTYPE_FIII_TL_SRTR		(0x08|0x40)
#define SB_DIMTYPE_FIIF_TL_SRTR		(0x09|0x40)
#define SB_DIMTYPE_FIFI_TL_SRTR		(0x0A|0x40)
#define SB_DIMTYPE_FIFF_TL_SRTR		(0x0B|0x40)
#define SB_DIMTYPE_FFII_TL_SRTR		(0x0C|0x40)
#define SB_DIMTYPE_FFIF_TL_SRTR		(0x0D|0x40)
#define SB_DIMTYPE_FFFI_TL_SRTR		(0x0E|0x40)
#define SB_DIMTYPE_FFFF_TL_SRTR		(0x0F|0x40)
#define SB_DIMTYPE_IIII_TR_SRTR		(0x10|0x40)
#define SB_DIMTYPE_IIIF_TR_SRTR		(0x11|0x40)
#define SB_DIMTYPE_IIFI_TR_SRTR		(0x12|0x40)
#define SB_DIMTYPE_IIFF_TR_SRTR		(0x13|0x40)
#define SB_DIMTYPE_IFII_TR_SRTR		(0x14|0x40)
#define SB_DIMTYPE_IFIF_TR_SRTR		(0x15|0x40)
#define SB_DIMTYPE_IFFI_TR_SRTR		(0x16|0x40)
#define SB_DIMTYPE_IFFF_TR_SRTR		(0x17|0x40)
#define SB_DIMTYPE_FIII_TR_SRTR		(0x18|0x40)
#define SB_DIMTYPE_FIIF_TR_SRTR		(0x19|0x40)
#define SB_DIMTYPE_FIFI_TR_SRTR		(0x1A|0x40)
#define SB_DIMTYPE_FIFF_TR_SRTR		(0x1B|0x40)
#define SB_DIMTYPE_FFII_TR_SRTR		(0x1C|0x40)
#define SB_DIMTYPE_FFIF_TR_SRTR		(0x1D|0x40)
#define SB_DIMTYPE_FFFI_TR_SRTR		(0x1E|0x40)
#define SB_DIMTYPE_FFFF_TR_SRTR		(0x1F|0x40)
#define SB_DIMTYPE_IIII_BL_SRTR		(0x20|0x40)
#define SB_DIMTYPE_IIIF_BL_SRTR		(0x21|0x40)
#define SB_DIMTYPE_IIFI_BL_SRTR		(0x22|0x40)
#define SB_DIMTYPE_IIFF_BL_SRTR		(0x23|0x40)
#define SB_DIMTYPE_IFII_BL_SRTR		(0x24|0x40)
#define SB_DIMTYPE_IFIF_BL_SRTR		(0x25|0x40)
#define SB_DIMTYPE_IFFI_BL_SRTR		(0x26|0x40)
#define SB_DIMTYPE_IFFF_BL_SRTR		(0x27|0x40)
#define SB_DIMTYPE_FIII_BL_SRTR		(0x28|0x40)
#define SB_DIMTYPE_FIIF_BL_SRTR		(0x29|0x40)
#define SB_DIMTYPE_FIFI_BL_SRTR		(0x2A|0x40)
#define SB_DIMTYPE_FIFF_BL_SRTR		(0x2B|0x40)
#define SB_DIMTYPE_FFII_BL_SRTR		(0x2C|0x40)
#define SB_DIMTYPE_FFIF_BL_SRTR		(0x2D|0x40)
#define SB_DIMTYPE_FFFI_BL_SRTR		(0x2E|0x40)
#define SB_DIMTYPE_FFFF_BL_SRTR		(0x2F|0x40)
#define SB_DIMTYPE_IIII_BR_SRTR		(0x30|0x40)
#define SB_DIMTYPE_IIIF_BR_SRTR		(0x31|0x40)
#define SB_DIMTYPE_IIFI_BR_SRTR		(0x32|0x40)
#define SB_DIMTYPE_IIFF_BR_SRTR		(0x33|0x40)
#define SB_DIMTYPE_IFII_BR_SRTR		(0x34|0x40)
#define SB_DIMTYPE_IFIF_BR_SRTR		(0x35|0x40)
#define SB_DIMTYPE_IFFI_BR_SRTR		(0x36|0x40)
#define SB_DIMTYPE_IFFF_BR_SRTR		(0x37|0x40)
#define SB_DIMTYPE_FIII_BR_SRTR		(0x38|0x40)
#define SB_DIMTYPE_FIIF_BR_SRTR		(0x39|0x40)
#define SB_DIMTYPE_FIFI_BR_SRTR		(0x3A|0x40)
#define SB_DIMTYPE_FIFF_BR_SRTR		(0x3B|0x40)
#define SB_DIMTYPE_FFII_BR_SRTR		(0x3C|0x40)
#define SB_DIMTYPE_FFIF_BR_SRTR		(0x3D|0x40)
#define SB_DIMTYPE_FFFI_BR_SRTR		(0x3E|0x40)
#define SB_DIMTYPE_FFFF_BR_SRTR		(0x3F|0x40)


#define SB_DIMTYPE_IIII_TL_SRBL		(0x00|0x80)
#define SB_DIMTYPE_IIIF_TL_SRBL		(0x01|0x80)
#define SB_DIMTYPE_IIFI_TL_SRBL		(0x02|0x80)
#define SB_DIMTYPE_IIFF_TL_SRBL		(0x03|0x80)
#define SB_DIMTYPE_IFII_TL_SRBL		(0x04|0x80)
#define SB_DIMTYPE_IFIF_TL_SRBL		(0x05|0x80)
#define SB_DIMTYPE_IFFI_TL_SRBL		(0x06|0x80)
#define SB_DIMTYPE_IFFF_TL_SRBL		(0x07|0x80)
#define SB_DIMTYPE_FIII_TL_SRBL		(0x08|0x80)
#define SB_DIMTYPE_FIIF_TL_SRBL		(0x09|0x80)
#define SB_DIMTYPE_FIFI_TL_SRBL		(0x0A|0x80)
#define SB_DIMTYPE_FIFF_TL_SRBL		(0x0B|0x80)
#define SB_DIMTYPE_FFII_TL_SRBL		(0x0C|0x80)
#define SB_DIMTYPE_FFIF_TL_SRBL		(0x0D|0x80)
#define SB_DIMTYPE_FFFI_TL_SRBL		(0x0E|0x80)
#define SB_DIMTYPE_FFFF_TL_SRBL		(0x0F|0x80)
#define SB_DIMTYPE_IIII_TR_SRBL		(0x10|0x80)
#define SB_DIMTYPE_IIIF_TR_SRBL		(0x11|0x80)
#define SB_DIMTYPE_IIFI_TR_SRBL		(0x12|0x80)
#define SB_DIMTYPE_IIFF_TR_SRBL		(0x13|0x80)
#define SB_DIMTYPE_IFII_TR_SRBL		(0x14|0x80)
#define SB_DIMTYPE_IFIF_TR_SRBL		(0x15|0x80)
#define SB_DIMTYPE_IFFI_TR_SRBL		(0x16|0x80)
#define SB_DIMTYPE_IFFF_TR_SRBL		(0x17|0x80)
#define SB_DIMTYPE_FIII_TR_SRBL		(0x18|0x80)
#define SB_DIMTYPE_FIIF_TR_SRBL		(0x19|0x80)
#define SB_DIMTYPE_FIFI_TR_SRBL		(0x1A|0x80)
#define SB_DIMTYPE_FIFF_TR_SRBL		(0x1B|0x80)
#define SB_DIMTYPE_FFII_TR_SRBL		(0x1C|0x80)
#define SB_DIMTYPE_FFIF_TR_SRBL		(0x1D|0x80)
#define SB_DIMTYPE_FFFI_TR_SRBL		(0x1E|0x80)
#define SB_DIMTYPE_FFFF_TR_SRBL		(0x1F|0x80)
#define SB_DIMTYPE_IIII_BL_SRBL		(0x20|0x80)
#define SB_DIMTYPE_IIIF_BL_SRBL		(0x21|0x80)
#define SB_DIMTYPE_IIFI_BL_SRBL		(0x22|0x80)
#define SB_DIMTYPE_IIFF_BL_SRBL		(0x23|0x80)
#define SB_DIMTYPE_IFII_BL_SRBL		(0x24|0x80)
#define SB_DIMTYPE_IFIF_BL_SRBL		(0x25|0x80)
#define SB_DIMTYPE_IFFI_BL_SRBL		(0x26|0x80)
#define SB_DIMTYPE_IFFF_BL_SRBL		(0x27|0x80)
#define SB_DIMTYPE_FIII_BL_SRBL		(0x28|0x80)
#define SB_DIMTYPE_FIIF_BL_SRBL		(0x29|0x80)
#define SB_DIMTYPE_FIFI_BL_SRBL		(0x2A|0x80)
#define SB_DIMTYPE_FIFF_BL_SRBL		(0x2B|0x80)
#define SB_DIMTYPE_FFII_BL_SRBL		(0x2C|0x80)
#define SB_DIMTYPE_FFIF_BL_SRBL		(0x2D|0x80)
#define SB_DIMTYPE_FFFI_BL_SRBL		(0x2E|0x80)
#define SB_DIMTYPE_FFFF_BL_SRBL		(0x2F|0x80)
#define SB_DIMTYPE_IIII_BR_SRBL		(0x30|0x80)
#define SB_DIMTYPE_IIIF_BR_SRBL		(0x31|0x80)
#define SB_DIMTYPE_IIFI_BR_SRBL		(0x32|0x80)
#define SB_DIMTYPE_IIFF_BR_SRBL		(0x33|0x80)
#define SB_DIMTYPE_IFII_BR_SRBL		(0x34|0x80)
#define SB_DIMTYPE_IFIF_BR_SRBL		(0x35|0x80)
#define SB_DIMTYPE_IFFI_BR_SRBL		(0x36|0x80)
#define SB_DIMTYPE_IFFF_BR_SRBL		(0x37|0x80)
#define SB_DIMTYPE_FIII_BR_SRBL		(0x38|0x80)
#define SB_DIMTYPE_FIIF_BR_SRBL		(0x39|0x80)
#define SB_DIMTYPE_FIFI_BR_SRBL		(0x3A|0x80)
#define SB_DIMTYPE_FIFF_BR_SRBL		(0x3B|0x80)
#define SB_DIMTYPE_FFII_BR_SRBL		(0x3C|0x80)
#define SB_DIMTYPE_FFIF_BR_SRBL		(0x3D|0x80)
#define SB_DIMTYPE_FFFI_BR_SRBL		(0x3E|0x80)
#define SB_DIMTYPE_FFFF_BR_SRBL		(0x3F|0x80)


#define SB_DIMTYPE_IIII_TL_SRBR		(0x00|0xC0)
#define SB_DIMTYPE_IIIF_TL_SRBR		(0x01|0xC0)
#define SB_DIMTYPE_IIFI_TL_SRBR		(0x02|0xC0)
#define SB_DIMTYPE_IIFF_TL_SRBR		(0x03|0xC0)
#define SB_DIMTYPE_IFII_TL_SRBR		(0x04|0xC0)
#define SB_DIMTYPE_IFIF_TL_SRBR		(0x05|0xC0)
#define SB_DIMTYPE_IFFI_TL_SRBR		(0x06|0xC0)
#define SB_DIMTYPE_IFFF_TL_SRBR		(0x07|0xC0)
#define SB_DIMTYPE_FIII_TL_SRBR		(0x08|0xC0)
#define SB_DIMTYPE_FIIF_TL_SRBR		(0x09|0xC0)
#define SB_DIMTYPE_FIFI_TL_SRBR		(0x0A|0xC0)
#define SB_DIMTYPE_FIFF_TL_SRBR		(0x0B|0xC0)
#define SB_DIMTYPE_FFII_TL_SRBR		(0x0C|0xC0)
#define SB_DIMTYPE_FFIF_TL_SRBR		(0x0D|0xC0)
#define SB_DIMTYPE_FFFI_TL_SRBR		(0x0E|0xC0)
#define SB_DIMTYPE_FFFF_TL_SRBR		(0x0F|0xC0)
#define SB_DIMTYPE_IIII_TR_SRBR		(0x10|0xC0)
#define SB_DIMTYPE_IIIF_TR_SRBR		(0x11|0xC0)
#define SB_DIMTYPE_IIFI_TR_SRBR		(0x12|0xC0)
#define SB_DIMTYPE_IIFF_TR_SRBR		(0x13|0xC0)
#define SB_DIMTYPE_IFII_TR_SRBR		(0x14|0xC0)
#define SB_DIMTYPE_IFIF_TR_SRBR		(0x15|0xC0)
#define SB_DIMTYPE_IFFI_TR_SRBR		(0x16|0xC0)
#define SB_DIMTYPE_IFFF_TR_SRBR		(0x17|0xC0)
#define SB_DIMTYPE_FIII_TR_SRBR		(0x18|0xC0)
#define SB_DIMTYPE_FIIF_TR_SRBR		(0x19|0xC0)
#define SB_DIMTYPE_FIFI_TR_SRBR		(0x1A|0xC0)
#define SB_DIMTYPE_FIFF_TR_SRBR		(0x1B|0xC0)
#define SB_DIMTYPE_FFII_TR_SRBR		(0x1C|0xC0)
#define SB_DIMTYPE_FFIF_TR_SRBR		(0x1D|0xC0)
#define SB_DIMTYPE_FFFI_TR_SRBR		(0x1E|0xC0)
#define SB_DIMTYPE_FFFF_TR_SRBR		(0x1F|0xC0)
#define SB_DIMTYPE_IIII_BL_SRBR		(0x20|0xC0)
#define SB_DIMTYPE_IIIF_BL_SRBR		(0x21|0xC0)
#define SB_DIMTYPE_IIFI_BL_SRBR		(0x22|0xC0)
#define SB_DIMTYPE_IIFF_BL_SRBR		(0x23|0xC0)
#define SB_DIMTYPE_IFII_BL_SRBR		(0x24|0xC0)
#define SB_DIMTYPE_IFIF_BL_SRBR		(0x25|0xC0)
#define SB_DIMTYPE_IFFI_BL_SRBR		(0x26|0xC0)
#define SB_DIMTYPE_IFFF_BL_SRBR		(0x27|0xC0)
#define SB_DIMTYPE_FIII_BL_SRBR		(0x28|0xC0)
#define SB_DIMTYPE_FIIF_BL_SRBR		(0x29|0xC0)
#define SB_DIMTYPE_FIFI_BL_SRBR		(0x2A|0xC0)
#define SB_DIMTYPE_FIFF_BL_SRBR		(0x2B|0xC0)
#define SB_DIMTYPE_FFII_BL_SRBR		(0x2C|0xC0)
#define SB_DIMTYPE_FFIF_BL_SRBR		(0x2D|0xC0)
#define SB_DIMTYPE_FFFI_BL_SRBR		(0x2E|0xC0)
#define SB_DIMTYPE_FFFF_BL_SRBR		(0x2F|0xC0)
#define SB_DIMTYPE_IIII_BR_SRBR		(0x30|0xC0)
#define SB_DIMTYPE_IIIF_BR_SRBR		(0x31|0xC0)
#define SB_DIMTYPE_IIFI_BR_SRBR		(0x32|0xC0)
#define SB_DIMTYPE_IIFF_BR_SRBR		(0x33|0xC0)
#define SB_DIMTYPE_IFII_BR_SRBR		(0x34|0xC0)
#define SB_DIMTYPE_IFIF_BR_SRBR		(0x35|0xC0)
#define SB_DIMTYPE_IFFI_BR_SRBR		(0x36|0xC0)
#define SB_DIMTYPE_IFFF_BR_SRBR		(0x37|0xC0)
#define SB_DIMTYPE_FIII_BR_SRBR		(0x38|0xC0)
#define SB_DIMTYPE_FIIF_BR_SRBR		(0x39|0xC0)
#define SB_DIMTYPE_FIFI_BR_SRBR		(0x3A|0xC0)
#define SB_DIMTYPE_FIFF_BR_SRBR		(0x3B|0xC0)
#define SB_DIMTYPE_FFII_BR_SRBR		(0x3C|0xC0)
#define SB_DIMTYPE_FFIF_BR_SRBR		(0x3D|0xC0)
#define SB_DIMTYPE_FFFI_BR_SRBR		(0x3E|0xC0)
#define SB_DIMTYPE_FFFF_BR_SRBR		(0x3F|0xC0)

// Returns a nonzero value on failiure.
int sbWndEvaluateLayout( sbWnd_Layout *, dimension[4] );

HWND
	HWNDCreator( const wString, sbWnd_Layout *, DWORD, DWORD, const wString, sbWnd * ),
	BasicWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),
	TextboxHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),

	BasicTextWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),
	ClickableWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),

	RestrictedImageWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),
	MasterWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),

	ViewcmdMasterWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),
	StringWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),

	ProgressBarWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),
	DldcmdMasterWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * ),

	VScrollbarWindowHWNDCreator( const wString, sbWnd_Layout *, sbWnd * );


sbWnd
	*BasicWindowCreator( const wString, sbWnd_Layout * ),
	*TextboxCreator( const wString, sbWnd_Layout *, int ),

	*BasicTextWindowCreator( const wString, sbWnd_Layout *, uint16_t ),
	*ClickableWindowCreator( const wString, sbWnd_Layout * ),

	*RestrictedImageWindowCreator( const wString, sbWnd_Layout *, const wString ),
	*MasterWindowCreator( const wString, sbWnd_Layout * ),

	*ViewcmdMasterWindowCreator( const wString, sbWnd_Layout * ),
	*StringWindowCreator( const wString, sbWnd_Layout *, const wString, uint16_t, int ),

	*ProgressBarWindowCreator( const wString, sbWnd_Layout *, uint16_t ),
	*DldcmdMasterWindowCreator( const wString, sbWnd_Layout * ),

	*VScrollbarWindowCreator( const wString, sbWnd_Layout *, uint16_t );


void
	destroy_AllTypes( sbWnd * ),
	sbWndCommonDestructor( sbWnd * );

void
	sbWnd_DeleteTextboxInfo( SBTextboxInfo * ),
	sbWnd_DeleteBasicTextWindowInfo( SBBasicTextWindowInfo * ),

	sbWnd_DeleteClickableWindowInfo( SBClickableWindowInfo * ),
	sbWnd_DeleteRestrictedImageWindowInfo( SBRestrictedImageWindowInfo * ),

	sbWnd_DeleteStringWindowInfo( SBStringWindowInfo * ),
	sbWnd_DeleteProgressBarWindowInfo( SBProgressBarWindowInfo * ),

	sbWnd_DeleteVScrollbarWindowInfo( SBVScrollbarWindowInfo * );

SBTextboxInfo *TextboxInfoCreator( HWND, int );
SBBasicTextWindowInfo *BasicTextWindowInfoCreator( HWND, uint16_t );
SBClickableWindowInfo *ClickableWindowInfoCreator( void );
SBRestrictedImageWindowInfo *RestrictedImageWindowInfoCreator( HWND, const wString );
SBStringWindowInfo *StringWindowInfoCreator( const wString, uint16_t, int );
SBProgressBarWindowInfo *ProgressBarWindowInfoCreator( uint16_t );
SBVScrollbarWindowInfo *VScrollbarWindowInfoCreator( uint16_t );

#endif