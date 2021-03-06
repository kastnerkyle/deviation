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

 Most of this code is based on the mixer from er9x developed by
 Erez Raviv <erezraviv@gmail.com>
 http://code.google.com/p/er9x/
 and the th9x project
 http://code.google.com/p/th9x/
 */

#include "common.h"
#include "mixer.h"
#include "config/model.h"

#define INPNAME_AILERON(x,y)  x = _tr_noop("AIL"); y = -1
#define INPNAME_ELEVATOR(x,y) x = _tr_noop("ELE"); y = -1
#define INPNAME_THROTTLE(x,y) x = _tr_noop("THR"); y = -1
#define INPNAME_RUDDER(x,y)   x = _tr_noop("RUD"); y = -1
#define INPNAME_AUX2(x,y)     x = _tr_noop("AUX2"); y = -1
#define INPNAME_AUX3(x,y)     x = _tr_noop("AUX3"); y = -1
#define INPNAME_AUX4(x,y)     x = _tr_noop("AUX4"); y = -1
#define INPNAME_AUX5(x,y)     x = _tr_noop("AUX5"); y = -1
#define INPNAME_AUX6(x,y)     x = _tr_noop("AUX6"); y = -1
#define INPNAME_AUX7(x,y)     x = _tr_noop("AUX7"); y = -1
#define INPNAME_DR0(x,y)      x = _tr_noop("DR"); y = 0
#define INPNAME_DR1(x,y)      x = _tr_noop("DR"); y = 1
#define INPNAME_RUD_DR0(x,y)  x = _tr_noop("RUD DR"); y = 0
#define INPNAME_RUD_DR1(x,y)  x = _tr_noop("RUD DR"); y = 1
#define INPNAME_RUD_DR2(x,y)  x = _tr_noop("RUD DR"); y = 2
#define INPNAME_ELE_DR0(x,y)  x = _tr_noop("ELE DR"); y = 0
#define INPNAME_ELE_DR1(x,y)  x = _tr_noop("ELE DR"); y = 1
#define INPNAME_ELE_DR2(x,y)  x = _tr_noop("ELE DR"); y = 2
#define INPNAME_AIL_DR0(x,y)  x = _tr_noop("AIL DR"); y = 0
#define INPNAME_AIL_DR1(x,y)  x = _tr_noop("AIL DR"); y = 1
#define INPNAME_AIL_DR2(x,y)  x = _tr_noop("AIL DR"); y = 2
#define INPNAME_GEAR0(x,y)    x = _tr_noop("GEAR"); y = 0
#define INPNAME_GEAR1(x,y)    x = _tr_noop("GEAR"); y = 1
#define INPNAME_MIX0(x,y)     x = _tr_noop("MIX"); y = 0
#define INPNAME_MIX1(x,y)     x = _tr_noop("MIX"); y = 1
#define INPNAME_MIX2(x,y)     x = _tr_noop("MIX"); y = 2
#define INPNAME_FMOD0(x,y)    x = _tr_noop("FMODE"); y = 0
#define INPNAME_FMOD1(x,y)    x = _tr_noop("FMODE"); y = 1
#define INPNAME_FMOD2(x,y)    x = _tr_noop("FMODE"); y = 2
#define INPNAME_HOLD0(x,y)    x = _tr_noop("HOLD"); y = 0
#define INPNAME_HOLD1(x,y)    x = _tr_noop("HOLD"); y = 1
#define INPNAME_TRN0(x,y)     x = _tr_noop("TRN"); y = 0
#define INPNAME_TRN1(x,y)     x = _tr_noop("TRN"); y = 1

#define SWITCH_NAME_GEAR0 _tr("GEAR")
#define SWITCH_NAME_GEAR1 _tr("GEAR")
#define SWITCH_NAME_GEAR0 _tr("GEAR")
#define BUTNAME_TRIM_LV_NEG _tr("TRIMLV-")
#define BUTNAME_TRIM_LV_POS _tr("TRIMLV+")
#define BUTNAME_TRIM_RV_NEG _tr("TRIMRV-")
#define BUTNAME_TRIM_RV_POS _tr("TRIMRV+")
#define BUTNAME_TRIM_LH_NEG _tr("TRIMLH-")
#define BUTNAME_TRIM_LH_POS _tr("TRIMLH+")
#define BUTNAME_TRIM_RH_NEG _tr("TRIMRH-")
#define BUTNAME_TRIM_RH_POS _tr("TRIMRH+")
#define BUTNAME_TRIM_L_NEG  _tr("TRIM_L-")
#define BUTNAME_TRIM_L_POS  _tr("TRIM_L+")
#define BUTNAME_TRIM_R_NEG  _tr("TRIM_R-")
#define BUTNAME_TRIM_R_POS  _tr("TRIM_R+")
#define BUTNAME_LEFT        _tr("Left")
#define BUTNAME_RIGHT       _tr("Right")
#define BUTNAME_DOWN        _tr("Down")
#define BUTNAME_UP          _tr("Up")
#define BUTNAME_ENTER       _tr("Enter")
#define BUTNAME_EXIT        _tr("Exit")

const char *tx_stick_names[4] = {
    _tr_noop("RIGHT_H"),
    _tr_noop("LEFT_V"),
    _tr_noop("RIGHT_V"),
    _tr_noop("LEFT_H"),
};

static void get_input_str(int src, const char **ptr, int *idx)
{
    *ptr = "";
    *idx = -1;
    #define CHANDEF(x) case INP_##x : INPNAME_##x(*ptr, *idx); break;
    switch(src) {
        #include "capabilities.h"
    };
    #undef CHANDEF
}

static const char *_get_source_name(char *str, u8 src, int switchname, int ignore_rename)
{
    u8 is_neg = MIXER_SRC_IS_INV(src);
    src = MIXER_SRC(src);

    if(! src) {
        strcpy(str, _tr("None"));
    } else if(src <= NUM_TX_INPUTS) {
        const char *ptr;
        int idx;
        get_input_str(src, &ptr, &idx);
        if(idx >= 0 && switchname)
            sprintf(str, "%s%s%d", is_neg ? "!" : "", _tr(ptr), idx);
        else
            sprintf(str, "%s%s", is_neg ? "!" : "", _tr(ptr));
    } else if(src <= NUM_INPUTS + NUM_OUT_CHANNELS) {
        sprintf(str, "%s%s%d", is_neg ? "!" : "", _tr("Ch"), src - NUM_INPUTS);
    } else if(src <= NUM_INPUTS + NUM_OUT_CHANNELS + NUM_VIRT_CHANNELS) {
        int virt = src - NUM_INPUTS - NUM_OUT_CHANNELS;
        if (! ignore_rename && Model.virtname[virt-1][0]) {
            sprintf(str, "%s%s", is_neg ? "!" : "", Model.virtname[virt-1]);
        } else {
            sprintf(str, "%s%s%d", is_neg ? "!" : "", _tr("Virt"), src - NUM_INPUTS - NUM_OUT_CHANNELS);
        }
    } else {
        sprintf(str, "%s%s%d", is_neg ? "!" : "", _tr("PPM"), src - NUM_INPUTS - NUM_OUT_CHANNELS - NUM_VIRT_CHANNELS);
    }
    return str;
}
const char *INPUT_SourceName(char *str, u8 src)
{
    return _get_source_name(str, src, 1, 0);
}
const char *INPUT_SourceNameReal(char *str, u8 src)
{
    // Use 'Virt' instead of renamed value
    return _get_source_name(str, src, 1, 1);
}
const char *INPUT_SourceNameAbbrevSwitch(char *str, u8 src)
{
    _get_source_name(str, src, 0, 0);
    return str;
}
const char *INPUT_SourceNameAbbrevSwitchReal(char *str, u8 src)
{
    // Use 'Virt' instead of renamed value
    _get_source_name(str, src, 0, 1);
    return str;
}

int INPUT_GetAbbrevSource(int origval, int newval, int dir)
{
    if (origval == newval || ! newval)
        return newval;
    int pos = INPUT_SwitchPos(newval);
    int num_pos = INPUT_NumSwitchPos(newval);
    if (num_pos != 0 && pos) {
        if (dir > 0)
            newval += (num_pos - pos);
        else
            newval -= pos;
    }
    return newval;
}

int INPUT_SwitchPos(u8 src)
{
    const char *ptr;
    int idx;
    get_input_str(src, &ptr, &idx);
    return idx;
}

int INPUT_NumSwitchPos(u8 src)
{
    const char *ptr, *ptr2;
    int idx, idx2;
    get_input_str(src, &ptr, &idx);
    if (idx == -1)
        return 0;
    while(src+1 < INP_LAST) {
        get_input_str(src+1, &ptr2, &idx2);
        if(ptr2 != ptr) {
            return idx+1;
        }
        idx = idx2;
        src++;
    }
    return idx+1;
}

int INPUT_GetFirstSwitch(int sw) {
    return sw - INPUT_SwitchPos(sw);
}

const char *INPUT_MapSourceName(u8 idx, u8 *val)
{
    int i = 0;
    #define CHANMAP(oldname, new) if(idx == i++) { *val = INP_##new; return oldname;}
    #include "capabilities.h"
    #undef CHANMAP
    return NULL;
}


const char *INPUT_ButtonName(u8 button)
{
    if (! button) {
        return _tr("None");
    }
    #define BUTTONDEF(x) case BUT_##x : return BUTNAME_##x;
    switch(button) {
        #include "capabilities.h"
    };
    #undef BUTTONDEF
    return "";
}
