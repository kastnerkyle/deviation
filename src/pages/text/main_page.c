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
#include "pages.h"
//#include "icons.h"
#include "gui/gui.h"
#include "config/model.h"
#include "config/tx.h"
#include "telemetry.h"

#define VTRIM_W       4
#define VTRIM_H      49
#define HTRIM_W      49
#define HTRIM_H       4
#define MODEL_ICO_W  52
#define MODEL_ICO_H  36
#define BOX_W        48
#define SMALLBOX_H    9
#define BIGBOX_H      9

#define ELEM_BIGBOX ELEM_SMALLBOX
#define ELEM_BAR

#define press_icon_cb NULL
#define press_box_cb NULL

#include "../common/_main_page.c"

#define BATTERY_SCAN_MSEC 2000 // refresh battery for every 2sec to avoid its label blinking
static u32 next_scan=0;

/*
 * Main page
 * KEY_UP: Press once to start timers, press again to stop timers
 * KEY_DOWN: Press to rest timers
 * KEY_ENT: enter the main menu page
 */
void PAGE_MainInit(int page)
{
    (void)page;
    (void)bar_cb;
    memset(mp, 0, sizeof(struct main_page));// Bug fix: must initialize this structure to avoid unpredictable issues in the PAGE_MainEvent
    memset(gui, 0, sizeof(struct mainpage_obj));
    PAGE_SetModal(0);
    PAGE_SetActionCB(_action_cb);
    PAGE_RemoveAllObjects();

    lcd_show_string(Model.name, 0, 0, 0);
    _check_voltage();
}

static void _check_voltage()
{
    static u8 video_on = 0;
    if (CLOCK_getms() > next_scan)  {  // don't need to check battery too frequently, to avoid blink of the battery label
        if(video_on == 0) {
            //LCD_ShowVideo();
            video_on = 1;
        }


        next_scan = CLOCK_getms() + BATTERY_SCAN_MSEC;
        s16 batt = PWR_ReadVoltage();

        // Creeate and show the string
        lcd_show_string(RADIO_TX_POWER_VAL[Model.tx_power], 0, -8, 0);

        // Blink battery level if low
        char batt_string[6];
        sprintf(batt_string, "%dmV", batt);
        if (batt < Transmitter.batt_alarm)
            lcd_show_string(batt_string, 0, -1, 0xFFFF);
        else
            lcd_show_string(batt_string, 0, -1, 0x0);
    }
}

void PAGE_MainExit()
{
}

static u8 _action_cb(u32 button, u8 flags, void *data)
{
    u8 i;
    if ((flags & BUTTON_PRESS) && CHAN_ButtonIsPressed(button, BUT_ENTER)) {
        u8 page = (0 << 4) | MENUTYPE_MAINMENU;
        PAGE_ChangeByID(PAGEID_MENU, page);
    } else if ((flags & BUTTON_PRESS) && CHAN_ButtonIsPressed(button, BUT_RIGHT)) {
	for ( i=0; i< NUM_TIMERS; i++) 
            TIMER_StartStop(i);
    } else if ((flags & BUTTON_PRESS) && CHAN_ButtonIsPressed(button, BUT_LEFT)) {
	for ( i=0; i< NUM_TIMERS; i++)
	    TIMER_Reset(i);
    } else if (! PAGE_QuickPage(button, flags, data)) {
        MIXER_UpdateTrim(button, flags, data);
    }
    return 1;
}
