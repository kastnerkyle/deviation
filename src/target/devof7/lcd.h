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

#ifndef LCD_H
#define LCD_H

#define LCD_ALIGN_LEFT      0
#define LCD_ALIGN_CENTER    1
#define LCD_ALIGN_RIGHT     2

void lcd_show_string(const char string[], u8 line, s8 pos);
void lcd_show_line(const char string[], u8 line, u8 align);

#endif //LCD_H