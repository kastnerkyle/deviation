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
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/spi.h>
#include "common.h"
#include "gui/gui.h"
#include "lcd.h"

#define CS_HI() gpio_set(GPIOB, GPIO0)
#define CS_LO() gpio_clear(GPIOB, GPIO0)

// The screen dimensions
#define LCD_SCREEN_LINES    11
#define LCD_SCREEN_CHARS    24
extern struct cur_str;

static unsigned char charmap[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //16
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //24
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //32
    0x00, 0x00, 0x00, 0x00, 0x0F, 0x0D, 0x0E, 0x69, //40
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, //48
    0x08, 0x09, 0x0A, 0x67, 0x0B, 0x00, 0x0C, 0x4D, //56
    0x00, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, //64
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, //72
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, //80
    0x28, 0x29, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, //88
    0x00, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, //96
    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, //104
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, //112
    0x68, 0x69, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x7E, //120
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //128
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //136
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //144
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //152
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //160
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //168
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //176
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //184
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //192
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //200
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //208
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //216
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //224
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //232
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //240
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //248
};

void LCD_Cmd(u8 cmd) {
    volatile int i;
    CS_LO();
    i = 300;
    while(i) i--;
    spi_xfer(SPI1, cmd);
    i = 300;
    while(i) i--;
    CS_HI();

    i = 2000;
    while(i) i--;
}

void LCD_CMDLength(u8 cmd[], u8 length) {
    volatile int i, j;
    CS_LO();
    i = 300;
    while(i) i--;
    for(i = 0; i < length; i++) {
    	spi_xfer(SPI1, cmd[i]);
    	j = 2000;
    	while(j) j--;
    }
    CS_HI();
    i = 300;
    while(i) i--;
}

void LCD_Contrast(u8 contrast)
{
    (void) contrast;
}

// Convert a string with the charmap
void lcd_convert_string(const char string[], u8 length, u8* output) {
    u8 i;
    for(i = 0; i < length; i++)
        output[i] = charmap[(u8)string[i]];
}

// Calculate string length
u8 lcd_string_lenght(const char string[]) {
    u8 i = 0;
    while(string[i] != 0 && i < LCD_SCREEN_CHARS) i++;
    return i;
}

// Show a string at a certain position
void lcd_show_string(const char string[], u8 line, s8 pos, u16 color) {
    u8 cmd[LCD_SCREEN_CHARS+2];
    u8 length = lcd_string_lenght(string);
    if(pos == -1)
        pos = LCD_SCREEN_CHARS-length;
    u16 position = (line << 5) + pos;

    // Check if it fits inside the screen
    if(line > LCD_SCREEN_LINES || pos+length > LCD_SCREEN_CHARS) //TODO
        return;

    // Send the position
    cmd[0] = 0x88 + (position >> 8);
    cmd[1] = position & 0xFF;
    LCD_CMDLength(cmd, 2);

    // Convert the string
    if(color != 0xC0 && color != 0xC2 && color != 0xC3)
        cmd[0] = 0xC0;
    else
        cmd[0] = color&0xFF; //C0->normal C2->flashing C3->flashing
    lcd_convert_string(string, length, &cmd[1]);
    cmd[length+1] = 0x7F;
    LCD_CMDLength(cmd, length+2);
}

// Show a string at a certain line
void lcd_show_line(const char string[], u8 line, u8 align, u16 color) {
    char new_string[LCD_SCREEN_CHARS];
    u8 pos_x, i, j;
    u8 length = lcd_string_lenght(string);

    // Check if it is inside the screen
    if(line > LCD_SCREEN_LINES || length > LCD_SCREEN_CHARS)
        return;

    // Calculate the X position
    if(align == LCD_ALIGN_LEFT)
        pos_x = 0;
    else if(align == LCD_ALIGN_CENTER)
        pos_x = (LCD_SCREEN_CHARS - length) / 2;
    else
        pos_x = (LCD_SCREEN_CHARS - length);

    // Create the new string
    j = 0;
    for(i = 0; i < LCD_SCREEN_CHARS; i++) {
        if(i < pos_x || pos_x+length <= i)
            new_string[i] = 0x7F; //del
        else {
            new_string[i] = string[j];
            j++;
        }
    }

    lcd_show_string(new_string, line, 0, color);
}

void LCD_Init()
{
    u8 cmd[2];
    /* Enable GPIOA clock. */
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);

    /* Set GPIO0, GPIO8, GPIO15, GPIO4 (in GPIO port A) to 'output push-pull'. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);  //CS0
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);  //CS1
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_PUSHPULL, GPIO15); //CS2
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_PUSHPULL, GPIO4);  //ON, OFF?

    // Set the 5.8GHz receiver on
    gpio_set(GPIOA, GPIO4);

    // Set the 5.8GHz channel
    gpio_clear(GPIOA, GPIO0);
    gpio_clear(GPIOA, GPIO8);
    gpio_clear(GPIOA, GPIO15);

    //Initialization is mostly done in SPI Flash
    //Setup CS as B.0 Data/Control = C.5
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);


    LCD_Cmd(0x00);

    volatile int i = 0x4FFFF;
    while(i) i--;

    LCD_Cmd(0x1E);
    LCD_Cmd(0x22);
    LCD_Cmd(0x32);
    LCD_Cmd(0x49);
    LCD_Cmd(0x52);

    cmd[0] = 0x90;
    cmd[1] = 0x07;
    LCD_CMDLength(cmd, 2);

    cmd[0] = 0xb0;
    cmd[1] = 0x00;
    LCD_CMDLength(cmd, 2);

    LCD_Cmd(0x45);

    cmd[0] = 0x80;
    cmd[1] = 0xc4;
    LCD_CMDLength(cmd, 2);

    for(i = 0; i < 0x0b; i++) {
    	cmd[0] = 0x98;
        cmd[1] = i;
        LCD_CMDLength(cmd, 2);
    }

    i = 0x8000;
    while(i) i--;

    LCD_Cmd(0x45);
}

void LCD_Clear(unsigned int val)
{
    u8 i;
    (void) val;

    for(i = 0; i <= LCD_SCREEN_LINES; i++)
        lcd_show_line("", i, LCD_ALIGN_LEFT, 0);
}

void LCD_DrawStart(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, enum DrawDir _dir)
{
    (void) x0; (void) y0; (void) x1; (void) y1; (void) _dir;
}

void LCD_DrawStop(void)
{
    
}

void LCD_DrawPixel(unsigned int color)
{
   (void) color;
}

void LCD_DrawPixelXY(unsigned int x, unsigned int y, unsigned int color)
{
    (void) x; (void) y; (void) color;
}
