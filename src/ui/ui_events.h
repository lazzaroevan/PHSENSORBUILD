// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: PHReaderWifiV1

#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

void loadDataButtonEvent(lv_event_t * e);
void wifiScanButtonEvent(lv_event_t * e);
void highPointCalButtonEvent(lv_event_t * e);
void lowPointCalButtonEvent(lv_event_t * e);
void midPointCalButtonEvent(lv_event_t * e);
void beginCalButtonEvent(lv_event_t * e);
void wifiTestButtonEvent(lv_event_t * e);
void wifiTest(lv_event_t * e);
void updateTempUnitsSwitchEvent(lv_event_t * e);
void resetDeviceButtonEvent(lv_event_t * e);
void screenOffButtonEvent(lv_event_t * e);
void setPlotTimePeriodDropdownEvent(lv_event_t * e);
void setSamplingIntervalDropdownEvent(lv_event_t * e);
void startBluetoothButtonEvent(lv_event_t * e);
void startWifiStaPageButtonEvent(lv_event_t * e);
void startWifiApPageButtonEvent(lv_event_t * e);
void closeWifiServerButtonEvent(lv_event_t * e);
void closeBluetoothButtonEvent(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
