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

#define CS_HI() gpio_set(GPIOB, GPIO0)
#define CS_LO() gpio_clear(GPIOB, GPIO0)

void LCD_Cmd(u8 cmd) {
    volatile int i;
    CS_LO();
    i = 3000;
    while(i) i--;
    spi_xfer(SPI1, cmd);
    i = 3000;
    while(i) i--;
    CS_HI();

    i = 20000;
    while(i) i--;
}

void LCD_CMDLength(u8 cmd[], u8 length) {
    volatile int i, j;
    CS_LO();
    i = 3000;
    while(i) i--;
    for(i = 0; i < length; i++) {
    	spi_xfer(SPI1, cmd[i]);
    	j = 20000;
    	while(j) j--;
    }
    CS_HI();
    i = 3000;
    while(i) i--;
}

void lcd_display(uint8_t on)
{
    
}

void lcd_set_page_address(uint8_t page)
{
    
}

void lcd_set_column_address(uint8_t column)
{
    
}

void lcd_set_start_line(int line)
{
  
}

void LCD_Contrast(u8 contrast)
{
    
}

void LCD_Init()
{
    u8 cmd[26];
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

    /*cmd[0] = 0x98;
    cmd[1] = 0x41;
    LCD_CMDLength(cmd, 2);*/



    cmd[0] = 0x88;
    cmd[1] = 0x00;
    LCD_CMDLength(cmd, 2);

    cmd[0] = 0xC0;
    for(i = 1; i < 25; i++)
    	cmd[i] = i-1;
    cmd[25] = 0x7F;
    LCD_CMDLength(cmd, 26);


    cmd[0] = 0x88;
	cmd[1] = 0x20;
	LCD_CMDLength(cmd, 2);

	cmd[0] = 0xC0;
	for(i = 1; i < 25; i++)
		cmd[i] = i+23;
	cmd[25] = 0x7F;
	LCD_CMDLength(cmd, 26);


	cmd[0] = 0x88;
	cmd[1] = 0x40;
	LCD_CMDLength(cmd, 2);

	cmd[0] = 0xC0;
	for(i = 1; i < 25; i++)
		cmd[i] = i+47;
	cmd[25] = 0x7F;
	LCD_CMDLength(cmd, 26);


	cmd[0] = 0x88;
	cmd[1] = 0x60;
	LCD_CMDLength(cmd, 2);

	cmd[0] = 0xC0;
	for(i = 1; i < 25; i++)
		cmd[i] = i+72;
	cmd[25] = 0x7F;
	LCD_CMDLength(cmd, 26);


	cmd[0] = 0x88;
	cmd[1] = 0x80;
	LCD_CMDLength(cmd, 2);

	cmd[0] = 0xC0;
	for(i = 1; i < 25; i++)
		cmd[i] = i+96;
	cmd[25] = 0x7F;
	LCD_CMDLength(cmd, 26);
}

void LCD_Clear(unsigned int val)
{
    
}

void LCD_DrawStart(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, enum DrawDir _dir)
{
    
}

void LCD_DrawStop(void)
{
    
}

void LCD_DrawPixel(unsigned int color)
{
   
}

void LCD_DrawPixelXY(unsigned int x, unsigned int y, unsigned int color)
{
    
}
