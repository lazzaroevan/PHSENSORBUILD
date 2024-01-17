// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: PHReaderWifiV1

#include "../ui.h"

void ui_configWifiScreen_screen_init(void)
{
    ui_configWifiScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_configWifiScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ssidLabelName = lv_label_create(ui_configWifiScreen);
    lv_obj_set_width(ui_ssidLabelName, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ssidLabelName, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ssidLabelName, -136);
    lv_obj_set_y(ui_ssidLabelName, -89);
    lv_obj_set_align(ui_ssidLabelName, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ssidLabelName, "SSID:");

    ui_passwordTextArea = lv_textarea_create(ui_configWifiScreen);
    lv_obj_set_width(ui_passwordTextArea, 194);
    lv_obj_set_height(ui_passwordTextArea, LV_SIZE_CONTENT);    /// 38
    lv_obj_set_x(ui_passwordTextArea, -11);
    lv_obj_set_y(ui_passwordTextArea, -37);
    lv_obj_set_align(ui_passwordTextArea, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_passwordTextArea, "PASSWORD");
    lv_textarea_set_one_line(ui_passwordTextArea, true);
    lv_textarea_set_password_mode(ui_passwordTextArea, true);



    ui_passLabelName = lv_label_create(ui_configWifiScreen);
    lv_obj_set_width(ui_passLabelName, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_passLabelName, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_passLabelName, -134);
    lv_obj_set_y(ui_passLabelName, -38);
    lv_obj_set_align(ui_passLabelName, LV_ALIGN_CENTER);
    lv_label_set_text(ui_passLabelName, "PASS:");

    ui_Keyboard = lv_keyboard_create(ui_configWifiScreen);
    lv_obj_set_width(ui_Keyboard, 313);
    lv_obj_set_height(ui_Keyboard, 126);
    lv_obj_set_x(ui_Keyboard, -1);
    lv_obj_set_y(ui_Keyboard, 55);
    lv_obj_set_align(ui_Keyboard, LV_ALIGN_CENTER);

    ui_setWifiInfoButton = lv_btn_create(ui_configWifiScreen);
    lv_obj_set_width(ui_setWifiInfoButton, 45);
    lv_obj_set_height(ui_setWifiInfoButton, 45);
    lv_obj_set_x(ui_setWifiInfoButton, 121);
    lv_obj_set_y(ui_setWifiInfoButton, -37);
    lv_obj_set_align(ui_setWifiInfoButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_setWifiInfoButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_setWifiInfoButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_setWifiInfoButton, lv_color_hex(0x3CCF49), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_setWifiInfoButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wifiTestButtonLabel = lv_label_create(ui_setWifiInfoButton);
    lv_obj_set_width(ui_wifiTestButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifiTestButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_wifiTestButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifiTestButtonLabel, "Save");
    lv_obj_set_style_text_color(ui_wifiTestButtonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_wifiTestButtonLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_wifiTestButtonLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wifiChooserDropdown = lv_dropdown_create(ui_configWifiScreen);
    lv_dropdown_set_options(ui_wifiChooserDropdown, "");
    lv_obj_set_width(ui_wifiChooserDropdown, 194);
    lv_obj_set_height(ui_wifiChooserDropdown, 38);
    lv_obj_set_x(ui_wifiChooserDropdown, -11);
    lv_obj_set_y(ui_wifiChooserDropdown, -88);
    lv_obj_set_align(ui_wifiChooserDropdown, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_wifiChooserDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags



    ui_RefreshScan = lv_btn_create(ui_configWifiScreen);
    lv_obj_set_width(ui_RefreshScan, 45);
    lv_obj_set_height(ui_RefreshScan, 45);
    lv_obj_set_x(ui_RefreshScan, 121);
    lv_obj_set_y(ui_RefreshScan, -88);
    lv_obj_set_align(ui_RefreshScan, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_RefreshScan, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_RefreshScan, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_RefreshScan, lv_color_hex(0x42AAFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RefreshScan, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_RefreshScan);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Scan");

    lv_obj_add_event_cb(ui_passwordTextArea, ui_event_passwordTextArea, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_wifiTestButtonLabel, ui_event_wifiTestButtonLabel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_setWifiInfoButton, ui_event_setWifiInfoButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_RefreshScan, ui_event_RefreshScan, LV_EVENT_ALL, NULL);

}
