SET(SOURCES screens/ui_EntranceScreen.c
    screens/ui_CalibratePhScreen.c
    screens/ui_configWifiScreen.c
    screens/ui_PHReaderMainScreen.c
    screens/ui_APorSTAScreen.c
    screens/ui_wifiTestScreen.c
    screens/ui_WifiApMode.c
    screens/ui_WifiStaMode.c
    screens/ui_bluetoothMode.c
    screens/ui_SleepScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp)

add_library(ui ${SOURCES})
