/*
    This project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Deviation is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "common.h"
#include "gui/gui.h"

struct FAT FontFAT;
/*
 * The font 'font_table' begins with a list of u24 values which represent
 * the offeset (from the beginning of the font file) of each character.
 * The font data follows immediately afterwards.
 *
 * The font data is represented as a bit-field.  A chunk of 1, 2, 3, or 4
 * bytes represents a single column of pixels.  This will be repeated for
 * the width of the font.
 * The column chunk can be thought of as a little-endian number of 8, 16, 24,
 * 32, 40, 48, 54 or 64 bits, with the low-order bit representing the top row,
 * and the 'height'-th bit representing the bottom row.
 *
 * Example: Here is a '!' as a 1x10 bit-field:
 *        *    1
 *        *    1
 *        *    1
 *        *    1
 *        *    1
 *        *    1
 *        *    1
 *             0   = 0x7F
 *        *    1
 *        *    1   = 0x03
 *
 *  So this would appear as '0x7F, 0x03' in the font table
 *
 */
//#define LINE_SPACING 2 // move to _gui.h as devo10's line spacing is different from devo8's
#define CHAR_SPACING 1
#define RANGE_TABLE_SIZE 20
#define NUM_FONTS 10

#define HEIGHT(x) x.height

 u8 FONT_GetFromString(const char * name) {
    (void) name;
    return 0;
 }

void LCD_PrintCharXY(unsigned int x, unsigned int y, u32 c)
{
    (void) x; (void) y; (void) c;
}

u8 LCD_SetFont(unsigned int idx)
{
    (void) idx;
    return 0;
}

u8 LCD_GetFont()
{
    return 0;
}

void LCD_SetXY(unsigned int x, unsigned int y)
{
    (void) x; (void) y;
}

void LCD_PrintStringXY(unsigned int x, unsigned int y, const char *str)
{
    (void) x; (void) y; (void) str;
}

void LCD_PrintString(const char *str)
{
    (void) str;
}

void LCD_PrintChar(u32 c)
{
    (void) c;
}

void LCD_GetCharDimensions(u32 c, u16 *width, u16 *height) {
    (void) c; (void) width; (void) height;
}

void LCD_GetStringDimensions(const u8 *str, u16 *width, u16 *height) {
    (void) str; (void) width; (void) height;
}

void LCD_SetFontColor(u16 color) {
    (void) color;
}
