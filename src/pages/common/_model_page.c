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

static struct model_page * const mp = &pagemem.u.model_page;

static void _changename_cb(guiObject_t *obj, const void *data);
static void fixedid_cb(guiObject_t *obj, const void *data);
static void bind_cb(guiObject_t *obj, const void *data);
static void configure_bind_button();
static const char *type_val_cb(guiObject_t *obj, int dir, void *data);
static void type_press_cb(guiObject_t *obj, void *data);
static const char *numchanselect_cb(guiObject_t *obj, int dir, void *data);
static const char *powerselect_cb(guiObject_t *obj, int dir, void *data);
static const char *protoselect_cb(guiObject_t *obj, int dir, void *data);
static const char *file_val_cb(guiObject_t *obj, int dir, void *data);
static void file_press_cb(guiObject_t *obj, void *data);
static void changeicon_cb(guiObject_t *obj, const void *data);

static void _changename_done_cb(guiObject_t *obj, void *data);

const char *show_text_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    int width; int height;
    u16 txt_w, txt_h;
    strcpy(mp->tmpstr, (const char *)data);
    GUI_GetSize(obj, &width, &height);
    width -=2;
    while(1) {
        LCD_GetStringDimensions((const u8 *)mp->tmpstr, &txt_w, &txt_h);
        if (txt_w > width) {
            int len = strlen(mp->tmpstr);
            if (mp->tmpstr[len-1] == '.')
                len--;
            mp->tmpstr[len-3] = '.';
            mp->tmpstr[len-2] = '.';
            mp->tmpstr[len-1] = '\0';
        } else {
            break;
        }
    }
    return mp->tmpstr;
}

const char *show_bindtext_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    (void)data;
    return PROTOCOL_AutoBindEnabled() ? _tr("Re-Init") : _tr("Bind");
}

void PAGE_ModelEvent()
{
}

static void fixedid_done_cb(guiObject_t *obj, void *data)
{
    (void)data;
    Model.fixed_id = atoi(mp->fixed_id);
    GUI_RemoveObj(obj);
    PAGE_ModelInit(-1); // must be -1 for devo10 to get back to correct page
}
static void fixedid_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    (void)data;
    PAGE_SetModal(1);
    if(Model.fixed_id == 0) {
        u32 id = PROTOCOL_CurrentID();
        if (id)
            sprintf(mp->fixed_id, "%d", (int)id);
        else
            mp->fixed_id[0] = 0;
    }
    PAGE_RemoveAllObjects();
    GUI_CreateKeyboard(KEYBOARD_NUM, mp->fixed_id, 999999, fixedid_done_cb, NULL);
}

static void bind_cb(guiObject_t *obj, const void *data)
{
    (void)data;
    (void)obj;
    if (PROTOCOL_AutoBindEnabled())
        PROTOCOL_Init(0);
    else
        PROTOCOL_Bind();
}

static void configure_bind_button()
{
    GUI_Redraw(mp->obj);
    //GUI_SetHidden(mp->obj, PROTOCOL_AutoBindEnabled());
}

/* Text Select Callback */
static const char *type_val_cb(guiObject_t *obj, int dir, void *data)
{
    (void)data;
    (void)obj;
    Model.type = GUI_TextSelectHelper(Model.type, 0, 1, dir, 1, 1, NULL);
    GUI_TextSelectEnablePress(obj, Model.type == 0);

    switch (Model.type) {
        case 0: return _tr(HELI_LABEL);
        default: return _tr(PLANE_LABEL);
    }
}
void type_press_cb(guiObject_t *obj, void *data)
{
    (void)data;
    (void)obj;
    if(Model.type == 0) {
        PAGE_RemoveAllObjects();
        MODELPAGE_Config();
    }
}
static const char *numchanselect_cb(guiObject_t *obj, int dir, void *data)
{
    (void)data;
    (void)obj;
    Model.num_channels = GUI_TextSelectHelper(Model.num_channels, 1, PROTOCOL_NumChannels(), dir, 1, 1, NULL);
    sprintf(mp->tmpstr, "%d", Model.num_channels);
    return mp->tmpstr;
}

static const char *powerselect_cb(guiObject_t *obj, int dir, void *data)
{
    (void)data;
    (void)obj;
    u8 changed;
    Model.tx_power = GUI_TextSelectHelper(Model.tx_power, TXPOWER_100uW, TXPOWER_LAST-1, dir, 1, 1, &changed);
    if (changed)
        PROTOCOL_SetPower();
    return RADIO_TX_POWER_VAL[Model.tx_power];
}
static const char *protoselect_cb(guiObject_t *obj, int dir, void *data)
{
    (void)data;
    (void)obj;
    u8 changed;
    Model.protocol = GUI_TextSelectHelper(Model.protocol, PROTOCOL_NONE, PROTOCOL_COUNT-1, dir, 1, 1, &changed);
    if (changed) {
        Model.num_channels = PROTOCOL_DefaultNumChannels();
        memset(Model.proto_opts, 0, sizeof(Model.proto_opts)); //This may cause an immediate change in behavior!
        GUI_Redraw(mp->chanObj);
        configure_bind_button();
    }
    GUI_TextSelectEnablePress(obj, PROTOCOL_GetOptions() ? 1 : 0);
    return ProtocolNames[Model.protocol];
}
void proto_press_cb(guiObject_t *obj, void *data)
{
    (void)data;
    (void)obj;
    if(PROTOCOL_GetOptions()) {
        PAGE_RemoveAllObjects();
        MODELPROTO_Config();
    }
}
static const char *file_val_cb(guiObject_t *obj, int dir, void *data)
{
    (void)data;
    (void)obj;
    mp->file_state = GUI_TextSelectHelper(mp->file_state, 0, 3, dir, 1, 1, NULL);
    if (mp->file_state == 0)
        return _tr("Load...");
    else if (mp->file_state == 1)
        return _tr("Copy To...");
    else if (mp->file_state == 2)
        return _tr("Template..");
    else if (mp->file_state == 3)
        return _tr("Reset");
    else
        return "";
}

static void file_press_cb(guiObject_t *obj, void *data)
{
    (void)obj;
    (void)data;
    if (mp->file_state == 3) {
        CONFIG_ResetModel();
        CONFIG_SaveModelIfNeeded();
        GUI_RedrawAllObjects();
    } else {
        PAGE_SetModal(1);
        MODELPage_ShowLoadSave(mp->file_state, PAGE_ModelInit);
    }
}

static void changeicon_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    (void)data;
    MODELPage_ShowLoadSave(3, PAGE_ModelInit);
}
