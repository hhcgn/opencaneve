#ifndef _FONTS_H
#define _FONTS_H

#include <stdint.h>

//
//  Structure used to define fonts
//
typedef struct {
    const uint8_t FontWidth;    /* Font width in pixels */
    uint8_t FontHeight;         /* Font height in pixels */
    const uint16_t *data;       /* Pointer to data font data array */
} FontDef;

//
//  Export the 3 available fonts
//
extern const	unsigned char F8X16s[];
extern const	unsigned char F6x8s[][6];
extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;
extern FontDef Font_16x16;
extern uint8_t Hzk[][16];
#endif  // _FONTS_H
