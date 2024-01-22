// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: PHReaderWifiV1

#include "../ui.h"

void ui_CalibratePhScreen_screen_init(void)
{
    ui_CalibratePhScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_CalibratePhScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_highPointCalButton = lv_btn_create(ui_CalibratePhScreen);
    lv_obj_set_width(ui_highPointCalButton, lv_pct(30));
    lv_obj_set_height(ui_highPointCalButton, lv_pct(30));
    lv_obj_set_x(ui_highPointCalButton, lv_pct(33));
    lv_obj_set_y(ui_highPointCalButton, lv_pct(30));
    lv_obj_set_align(ui_highPointCalButton, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_highPointCalButton, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_highPointCalButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_highPointCalButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_highPointCalLabel = lv_label_create(ui_highPointCalButton);
    lv_obj_set_width(ui_highPointCalLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_highPointCalLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_highPointCalLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_highPointCalLabel, "HIGH");
    lv_obj_set_style_text_font(ui_highPointCalLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lowPointCalButton = lv_btn_create(ui_CalibratePhScreen);
    lv_obj_set_width(ui_lowPointCalButton, lv_pct(30));
    lv_obj_set_height(ui_lowPointCalButton, lv_pct(30));
    lv_obj_set_x(ui_lowPointCalButton, lv_pct(0));
    lv_obj_set_y(ui_lowPointCalButton, lv_pct(30));
    lv_obj_set_align(ui_lowPointCalButton, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_lowPointCalButton, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_lowPointCalButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_lowPointCalButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_lowPointCalButton, lv_color_hex(0xC51C1C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_lowPointCalButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lowPointCalLabel = lv_label_create(ui_lowPointCalButton);
    lv_obj_set_width(ui_lowPointCalLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lowPointCalLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lowPointCalLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lowPointCalLabel, "LOW");
    lv_obj_set_style_text_font(ui_lowPointCalLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_midPointCalButton = lv_btn_create(ui_CalibratePhScreen);
    lv_obj_set_width(ui_midPointCalButton, lv_pct(30));
    lv_obj_set_height(ui_midPointCalButton, lv_pct(30));
    lv_obj_set_x(ui_midPointCalButton, lv_pct(-33));
    lv_obj_set_y(ui_midPointCalButton, lv_pct(30));
    lv_obj_set_align(ui_midPointCalButton, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_midPointCalButton, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_midPointCalButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_midPointCalButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_midPointCalButton, lv_color_hex(0xF3F12D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_midPointCalButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_midPointCalLabel = lv_label_create(ui_midPointCalButton);
    lv_obj_set_width(ui_midPointCalLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_midPointCalLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_midPointCalLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_midPointCalLabel, "MID");
    lv_obj_set_style_text_color(ui_midPointCalLabel, lv_color_hex(0x010000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_midPointCalLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_midPointCalLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_beginCalButton = lv_btn_create(ui_CalibratePhScreen);
    lv_obj_set_width(ui_beginCalButton, lv_pct(96));
    lv_obj_set_height(ui_beginCalButton, lv_pct(25));
    lv_obj_set_align(ui_beginCalButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_beginCalButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_beginCalButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_beginCalButton, lv_color_hex(0x07F725), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_beginCalButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_beginCalLabel = lv_label_create(ui_beginCalButton);
    lv_obj_set_width(ui_beginCalLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_beginCalLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_beginCalLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_beginCalLabel, "Begin Calibration");
    lv_obj_set_style_text_font(ui_beginCalLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_calibrationLabel = lv_label_create(ui_CalibratePhScreen);
    lv_obj_set_width(ui_calibrationLabel, lv_pct(94));
    lv_obj_set_height(ui_calibrationLabel, lv_pct(25));
    lv_obj_set_x(ui_calibrationLabel, 0);
    lv_obj_set_y(ui_calibrationLabel, -73);
    lv_obj_set_align(ui_calibrationLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_calibrationLabel,
                      "To begin calibration, place the probe into the midpoint solution and press the big green button. Please Do not turn off the device while calibrating.");
    lv_obj_set_style_text_align(ui_calibrationLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_calibrationLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_highPointCalButton, ui_event_highPointCalButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_lowPointCalButton, ui_event_lowPointCalButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_midPointCalButton, ui_event_midPointCalButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_beginCalButton, ui_event_beginCalButton, LV_EVENT_ALL, NULL);

}
