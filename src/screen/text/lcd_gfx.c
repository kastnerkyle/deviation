/************************************
This is a our graphics core library, for all our displays. 
We'll be adapting all the
existing libaries to use this core to make updating, support 
and upgrading easier!

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution
****************************************/


#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include "gui/gui.h"
#include "lcd.h"

/* helper functions */

#define swap(x, y) {int __tmp = x; x = y; y = __tmp;}

void LCD_DrawFastVLine(int16_t x, int16_t y, 
                 int16_t h, uint16_t color) {
   
}

void LCD_DrawFastHLine(u16 x, u16 y, u16 w, u16 color) {
   
}

void LCD_DrawDashedHLine(int16_t x, int16_t y, 
             int16_t w, int16_t space, uint16_t color)
{
   
}

void LCD_DrawDashedVLine(int16_t x, int16_t y, 
             int16_t h, int16_t space, uint16_t color) {
    
}

// used to do circles and roundrects!
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                uint8_t cornername, int16_t delta, uint16_t color) {

}

void drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {

}

/* end of helper functions */

// draw a circle outline
void LCD_DrawCircle(u16 x0, u16 y0, u16 r, u16 color)
{
 
}

void LCD_FillCircle(u16 x0, u16 y0, u16 r, u16 color)
{
   
}


// bresenham's algorithm - thx wikpedia
void LCD_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 color)
{
 
  
}

// draw a rectangle
void LCD_DrawRect(u16 x, u16 y, u16 w, u16 h, u16 color)
{
 
}

void LCD_FillRect(u16 x, u16 y, u16 w, u16 h, u16 color)
{
   
}

// draw a rounded rectangle!
void LCD_DrawRoundRect(u16 x, u16 y, u16 w, u16 h, u16 r, u16 color)
{
 
}

// fill a rounded rectangle!
void LCD_FillRoundRect(u16 x, u16 y, u16 w, u16 h, u16 r, u16 color)
{
 
}

// draw a triangle!
void LCD_DrawTriangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
  
}

// fill a triangle!
void LCD_FillTriangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
 
}

u8 LCD_ImageIsTransparent(const char *file)
{
   return 1;
}

u8 LCD_ImageDimensions(const char *file, u16 *w, u16 *h)
{
    return 1;
}

void LCD_DrawWindowedImageFromFile(u16 x, u16 y, const char *file, s16 w, s16 h, u16 x_off, u16 y_off)
{

}

void LCD_DrawImageFromFile(u16 x, u16 y, const char *file)
{
    
}

void LCD_DrawRLE(const u8 *data, int len, u32 color)
{
   
}

void LCD_DrawUSBLogo(int lcd_width, int lcd_height)
{
   lcd_show_line("TU Delft MAVLab", 0, LCD_ALIGN_CENTER, 0);
   lcd_show_line("USB Mode", 1, LCD_ALIGN_CENTER, 0xC3);
   lcd_show_line("c Freek van Tienen", 11, LCD_ALIGN_CENTER, 0);
}
