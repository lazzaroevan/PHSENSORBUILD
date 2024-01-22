#include <Arduino.h>
#include <string.h>
#include <map>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <onewire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <fileServer.h>


TaskHandle_t wifiAPTaskHandle;

char WIFI_SSID[64];
char WIFI_PASSWORD[64];
//#define WIFI_SSID "Achilles1G"
//#define WIFI_PASSWORD "Odysseus277"
#define address 99                                         //default I2C ID number for EZO pH Circuit.
const int inputOnlyRx = 35;
const int useableTx = 22;
const int tempSensPin = 27;
//const int csPin = 5;                                       //SD card CS pin

ulong next_millis;
bool curUnitCelcius;
bool wifiServerUp;
bool tempSenseActive = true;
bool phSenseActive = true;
bool midCal = false;
bool lowCal = false;
bool highCal = false;
bool calibrating = false;
int buttCount;
char wifiTestLabel[64];
char tempBuffer[7];
char phBuffer[7];
char phCalLabelBuff[64];
int samplingInterval = 100;
float tempVal = 0;
float phVal = 0;
float celciusTemp = 0;
char ipCharArray[100];
//from ph sensor
byte serial_event = 0;           //a flag to signal when data has been received from the pc/mac/other.
byte received_from_computer = 0; //we need to know how many characters have been received.
char computerdata[20];           //we make a 20 byte character array to hold incoming data from a pc/mac/other.
const char * inputstring;                              //a string to hold incoming data from the PC
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
char strclear[100];



void serialEvent()                                                               //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.
{                                                                                //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13).c_str();                                      //read the string until we see a <CR>
  input_string_complete = true;                                                  //set the flag used to tell if we have received a completed string from the PC
}

void sendPHCode(String code)
{
    int baudRate = 9600;
    Serial2.begin(baudRate, SERIAL_8N1, inputOnlyRx, useableTx);
    Serial2.print("\r");
    Serial2.print("*OK,0\r");
    delay(100);
    Serial2.print(code);
    Serial2.end();
    delay(100);
}

void phSenseGetVal(void *pvParameters)
{
    Serial.println("PHSENSE VTASK BEGINING");
    int baudRate = 9600;
    Serial2.begin(baudRate, SERIAL_8N1, inputOnlyRx, useableTx);
    String sensorstring;                             //a string to hold the data from the Atlas Scientific product
    Serial2.print("\r");
    Serial2.print("*OK,0\r");
    Serial2.print("C,0\r");
    Serial2.print("L,0\r");
    Serial2.print("\r");
    while(phSenseActive)
    {
        delay(1000);
        Serial2.print("RT,");
        Serial2.print(celciusTemp);
        Serial2.print("\r");
        while(!sensor_string_complete)
        {   
            if (Serial2.available() > 0) 
            {                     //if we see that the Atlas Scientific product has sent a character
                char inchar = (char)Serial2.read();              //get the char we just received
                sensorstring += inchar;                           //add the char to the var called sensorstring
                if (inchar == '\r') 
                {                             //if the incoming character is a <CR>
                    sensor_string_complete = true;                  //set the flag
                }
            }
            if (sensor_string_complete == true) 
            {               //if a string from the Atlas Scientific product has been received in its entirety
                phVal = sensorstring.toFloat();
                int ret = snprintf(phBuffer, 6, "%f", phVal);
                sensorstring = "";                                //clear the string
            }
        }
        sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    }
    Serial2.end();
    Serial.println("PHSENSE VTASK ENDING");
    vTaskDelete(NULL);
}

void calibration(void *pvParameters)
{
    calibrating = true;
    TaskHandle_t phCalTaskHandle = NULL;
    xTaskCreate(phSenseGetVal, "phSenseLoop", 20000, NULL, tskIDLE_PRIORITY, &phCalTaskHandle);
    delay(1000);
    char strPH[100];
    strcpy(strPH,"When PH Value stabalises, press the MID button.\nPH: ");
    lv_obj_add_state(ui_beginCalButton, LV_STATE_DISABLED);
    lv_obj_clear_state(ui_midPointCalButton,LV_STATE_DISABLED);
    char phCalLabelBuffTemp[100];
    while (!midCal)
    {
        for(int i = 0;i<sizeof(strPH);i++)
        {
            phCalLabelBuffTemp[i]=strPH[i];
        }
        for(int i = 0;i<7;i++)
        {
            phCalLabelBuffTemp[i+sizeof(strPH)]=phBuffer[i];
        }
        strncpy(phCalLabelBuff,phCalLabelBuffTemp,sizeof(phCalLabelBuff));
    }
    phSenseActive = false; //Setting this false will delete the task
    while (eTaskGetState(phCalTaskHandle) != eDeleted) //wait for task to be deleted
    {
        delay(100);
    }
    phSenseActive = true;
    sendPHCode("Cal,mid,7.00\r"); //Do the midpoint calibration
    xTaskCreate(phSenseGetVal, "phSenseLoop", 20000, NULL, tskIDLE_PRIORITY, &phCalTaskHandle);
    memcpy(strPH,strclear,sizeof(strPH));
    strcat(strPH,"When PH Value stabalises, press the LOW button.\nPH: ");
    lv_obj_clear_state(ui_lowPointCalButton,LV_STATE_DISABLED);
    lv_obj_add_state(ui_midPointCalButton, LV_STATE_DISABLED);
    while (!lowCal)
    {
        for(int i = 0;i<sizeof(strPH);i++)
        {
            phCalLabelBuffTemp[i]=strPH[i];
        }
        for(int i = 0;i<7;i++)
        {
            phCalLabelBuffTemp[i+sizeof(strPH)]=phBuffer[i];
        }
        strncpy(phCalLabelBuff,phCalLabelBuffTemp,64);
    }
    phSenseActive = false; //Setting this false will delete the task
    while (eTaskGetState(phCalTaskHandle) != eDeleted) //wait for task to be deleted
    {
        delay(100);
    }
    phSenseActive = true;
    sendPHCode("Cal,low,4.00\r"); //Do the lowpoint calibration
    xTaskCreate(phSenseGetVal, "phSenseLoop", 20000, NULL, tskIDLE_PRIORITY, &phCalTaskHandle);
    memcpy(strPH,strclear,sizeof(strPH));
    strcat(strPH,"When PH Value stabalises, press the HIGH button.\nPH: ");
    lv_obj_clear_state(ui_highPointCalButton,LV_STATE_DISABLED);
    lv_obj_add_state(ui_lowPointCalButton, LV_STATE_DISABLED);
    while (!highCal)
    {
        for(int i = 0;i<sizeof(strPH);i++)
        {
            phCalLabelBuffTemp[i]=strPH[i];
        }
        for(int i = 0;i<7;i++)
        {
            phCalLabelBuffTemp[i+sizeof(strPH)]=phBuffer[i];
        }
        strncpy(phCalLabelBuff,phCalLabelBuffTemp,64);
    }
    phSenseActive = false; //Setting this false will delete the task
    while (eTaskGetState(phCalTaskHandle) != eDeleted) //wait for task to be deleted
    {
        delay(100);
    }
    phSenseActive = true;
    sendPHCode("Cal,high,10.00\r"); //Do the highpoint calibration
    calibrating = false;
    lv_label_set_text(ui_calibrationLabel,"Device Calibrated");
    lv_obj_set_style_text_font(ui_calibrationLabel, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
    delay(3000);
    lv_obj_set_style_text_font(ui_calibrationLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lowCal = false;
    highCal = false;
    midCal = false;
    _ui_screen_change(&ui_EntranceScreen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_EntranceScreen_screen_init);
    vTaskDelete(NULL);
}

void tempSenseGetVal(void *pvParameters)
{
    OneWire oneWire(tempSensPin);
    DallasTemperature sensors(&oneWire);
    sensors.begin();
    while(tempSenseActive)
    {
        //Serial.print("Requesting temperatures...");
        sensors.requestTemperatures(); // Send the command to get temperatures
        //Serial.println("DONE");

        //Serial.print("Temperature for Device 1 is: ");
        //Serial.print(sensors.getTempCByIndex(0));
        tempVal = sensors.getTempCByIndex(0);
        celciusTemp = tempVal;
        if(!curUnitCelcius)
        {
            tempVal = tempVal *1.800 + 32;
        }
        //char * degString = "\xB0";
        snprintf(tempBuffer, 6, "%f", tempVal);
        //tempBuffer[6] = degString[0];
        //Serial.println(buffer);
        if(samplingInterval > 110)
        {
            delay(samplingInterval);
        }
        
    }
    vTaskDelete(NULL);
}

void setup()
{
    delay(250);
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());

    smartdisplay_init();
    
    static auto disp = lv_disp_get_default();
    lv_disp_set_rotation(disp, LV_DISP_ROT_270);
    smartdisplay_lcd_set_brightness_cb(NULL, 1000);
    smartdisplay_lcd_set_backlight(1);
    ui_init();

    //Setting Initial Variable Values
    bool curUnitCelcius = false;
    buttCount = 0;
    //lv_obj_set_style_max_width();
    lv_obj_set_width(lv_dropdown_get_list(ui_wifiChooserDropdown), 260);
}

void loop()
{
    if(calibrating)
    {
        lv_label_set_text(ui_calibrationLabel,phCalLabelBuff);
    }
    else
    {
        lv_label_set_text(ui_tempValLabel,tempBuffer);
        lv_label_set_text(ui_ipLabelApMode, ipCharArray);
        lv_label_set_text(ui_ipLabelStaMode, ipCharArray);
        lv_label_set_text(ui_pHValLabel,phBuffer);
        lv_label_set_text(ui_wifiTestLabel,wifiTestLabel);
    }
    lv_timer_handler();
}

void loadDataButtonEvent(lv_event_t * e)
{
    loadData();
}
void loadData()
{
    Serial.println("Loading Data...");
    if(!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
    }
    else
    {
        if(SPIFFS.exists("/settings.txt"))
        { 
            File settingsFile = SPIFFS.open("/settings.txt");
            if(settingsFile)
            {
                /*
                int sizeOfSettings = settingsFile.size();
                int countRow = 0;
                int countCol = 0;
                int biggestRow = 0;
                char ** settingsToSet;
                char settingsCharArray[sizeOfSettings];
                settingsFile.read((uint8_t *)settingsCharArray, sizeOfSettings);
                for(int i = 0; i< sizeOfSettings; i++)
                {
                    if(settingsCharArray[i] == ' ')
                    {
                        countRow++; 
                        if(biggestRow<countCol)
                        {
                            biggestRow = countCol;
                        }
                        countCol = 0;   
                    }
                    else
                    {
                        countCol++;
                    }
                }
                char settingsDoubleArray[countRow][biggestRow];
                int row = 0;
                for(int i = 0; i< sizeOfSettings; i++)
                {
                    if(settingsCharArray[i] == ' ')
                    {
                        row++;
                    }
                    else
                    {
                        settingsDoubleArray[countRow][i]=settingsCharArray[i];
                    }
                }
                for(int i = 0; i < countRow; i++)
                {
                    int indexOfSpace = 0;
                    for(int n = 0; n< sizeof(settingsDoubleArray[i]);i++)
                    {
                        if(settingsDoubleArray[i][n] == ' ')
                        {
                            indexOfSpace = n;
                        }
                    }
                    char compareBuff[indexOfSpace];
                    char settingValBuff[sizeof(settingsDoubleArray[i])-indexOfSpace];
                    memcpy(compareBuff,settingsDoubleArray[i],indexOfSpace);
                    if(strcmp(compareBuff,"SampleingInt"))
                    {

                    }
                    
                }*/
            }
            else
            {
                Serial.println("Ooops Error");
            }
        }
        else
        {
            File settingsFile = SPIFFS.open("/settings.txt",FILE_WRITE);
            settingsFile.print("sampleingInt:1 ");
            settingsFile.print("graphInt:1 ");
            settingsFile.print("SSID: ");
            settingsFile.print("PASSWORD: ");
            settingsFile.close();
        }
    }
    xTaskCreate(tempSenseGetVal, "tempSenseLoop", 20000, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(phSenseGetVal, "phSenseLoop", 20000, NULL, tskIDLE_PRIORITY, NULL);
}

void updateTempUnitsSwitchEvent(lv_event_t * e)
{
    updateTempUnits();
}
void updateTempUnits()
{
    curUnitCelcius = lv_obj_has_state(ui_tempUnitsSwitch, LV_STATE_CHECKED);
	if(curUnitCelcius)
    {
        Serial.println("Changing unit from: Fahrenheit to Celcius ...");
    }
    else
    {
        Serial.println("Changing unit from: Celcius to Fahrenheit ...");
    }
    Serial.println("Success!");
}

void setPlotTimePeriodDropdownEvent(lv_event_t * e)
{
	setPlotTimePeriod(lv_dropdown_get_selected(ui_plotTimePeriodDropdown));
}
void setPlotTimePeriod(int valSel)
{
	Serial.println("Setting Plot Time Period...");
    switch (valSel)
    {
        case 0:
            Serial.println("Set Graph for PH and Temp plots to 1 Minute");
            break;
        case 1:
            Serial.println("Set Graph for PH and Temp plots to 5 Mintues");
            break;
        case 2:
            Serial.println("Set Graph for PH and Temp plots to 30 Minutes");
            break;
        case 3:
           Serial.println("Set Graph for PH and Temp plots to 1 Hour");
           break;
        case 4:
           Serial.println("Set Graph for PH and Temp plots to 2 Hours");
           break;
        case 5:
           Serial.println("Set Graph for PH and Temp plots to 1 Day");
           break;
        default:
            break;
    }
    Serial.println("Success!");
}

void setSamplingIntervalDropdownEvent(lv_event_t * e)
{
	setSamplingInterval(lv_dropdown_get_selected(ui_samplingIntervalDropdown));
}
void setSamplingInterval(int valSel)
{
	Serial.println("Setting Sampling Interval...");
    switch (valSel)
    {
        case 0:
            Serial.println("Set PH and temp Reading Interval to .1 Seconds");
            samplingInterval = 100;
            break;
        case 1:
            Serial.println("Set PH and temp Reading Interval to .5 Seconds");
            samplingInterval = 500;
            break;
        case 2:
            Serial.println("Set PH and temp Reading Interval to 1 Second");
            samplingInterval = 1000;
            break;
        case 3:
            Serial.println("Set PH and temp Reading Interval to 5 Seconds");
            samplingInterval = 5000;
            break;
        case 4:
            Serial.println("Set PH and temp Reading Interval to 30 Seconds");
            samplingInterval = 30000;
            break;
        default:
            break;
    }
    Serial.println("Success!");
}

void wifiScanButtonEvent(lv_event_t * e)
{
    xTaskCreate(wifiScan, "wifiScan", 20000, NULL, tskIDLE_PRIORITY, NULL);
}
void wifiScan(void *pvParameters)
{
	WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    String ssids;
    String ssidArray[n];
    int32_t rssiArray[n];
    int32_t tempInt;
    int biggestIndex;
    String tempString;
    for(int i = 0; i < n; i++)
    {
        ssidArray[i]=WiFi.SSID(i).c_str();
        rssiArray[i]=WiFi.RSSI(i);
    } 
    for(int i = 0; i < n-1; i++)
    {
        Serial.println(rssiArray[i]);
        for(int c = i; c<n; c++)
        {
            if(rssiArray[i]<=rssiArray[c])
            {
                tempInt = rssiArray[i];
                tempString = ssidArray[i];
                rssiArray[i]=rssiArray[c];
                ssidArray[i]=ssidArray[c];
                rssiArray[c]=tempInt;
                ssidArray[c]=tempString;
            }
        }
    }
    for(int i = n;i-->0;)
    {
        if(i == 0)
        {
            ssids += ssidArray[n-i-1];
        }
        else
        {
            ssids += ssidArray[n-i-1]+"\n";
        }
    }
    Serial.println("Assigning SSIDs to dropdown");
    Serial.println(ssids);
    lv_dropdown_set_options(ui_wifiChooserDropdown,ssids.c_str());
    vTaskDelete(NULL);
}

void closeWifiServerButtonEvent(lv_event_t * e)
{
	closeWifiServer();
}
void closeWifiServer()
{
    WiFi.mode(WIFI_MODE_STA);
    wifiServerUp = false;
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    Serial.println("Success!");
}

void startWifiApPageButtonEvent(lv_event_t * e)
{
	wifiServerUp = true;
    xTaskCreate(wifiAPLoop, "wifiAPLoop", 20000, NULL, tskIDLE_PRIORITY, &wifiAPTaskHandle);
}
void wifiAPLoop(void *pvParameters)
{
    Serial.println("Starting Server....");
    char connectingString[64];
    strcat(connectingString,"Creating access point....");
    memcpy(ipCharArray,connectingString,sizeof(ipCharArray));
    char tempIPString[64];
    strcat(tempIPString,"Connect to the PHSENSOR WiFi and navigate to: \"");
    strcat(tempIPString,serverSetup(WIFI_SSID,WIFI_PASSWORD,true).c_str());
    strcat(tempIPString, "\" in your browser.");
    memcpy(ipCharArray,tempIPString,sizeof(ipCharArray));
    while(wifiServerUp)
    {
        serverLoop(celciusTemp,phVal);
    }
    Serial.println("Closing Server....");
    Serial.println("Closing Access Point....");
    vTaskDelete(NULL);
}

void startWifiStaPageButtonEvent(lv_event_t * e)
{
	wifiServerUp = true;
    xTaskCreate(wifiSTALoop, "wifiSTALoop", 20000, NULL, tskIDLE_PRIORITY, &wifiAPTaskHandle);
}
void wifiSTALoop(void *pvParameters)
{
    Serial.println("Starting Server....");
    char connectingString[64];
    strcat(connectingString,"Connecting to: ");
    strcat(connectingString,WIFI_SSID);
    memcpy(ipCharArray,connectingString,sizeof(ipCharArray));
    char tempIPString[64] = "Connected to :";
    strcat(tempIPString, WIFI_SSID);
    strcat(tempIPString,". Navigate to: \"");
    strcat(tempIPString,serverSetup(WIFI_SSID,WIFI_PASSWORD,false).c_str());
    strcat(tempIPString,"\" in your browser.");
    memcpy(ipCharArray,tempIPString,sizeof(ipCharArray));
    while(wifiServerUp)
    {
        serverLoop(celciusTemp,phVal);
    }
    Serial.println("Closing Server....");
    
    vTaskDelete(NULL);
}

void highPointCalButtonEvent(lv_event_t * e)
{
	highPointCal();
}
void highPointCal()
{
	highCal = true;
}

void lowPointCalButtonEvent(lv_event_t * e)
{
	lowPointCal();
}
void lowPointCal()
{
	lowCal = true;
}

void midPointCalButtonEvent(lv_event_t * e)
{
	midPointCal();
}
void midPointCal()
{
	midCal = true;
}

void beginCalButtonEvent(lv_event_t * e)
{
	beginCal();
}
void beginCal()
{
	xTaskCreate(calibration, "calibration", 20000, NULL, tskIDLE_PRIORITY, NULL);
}

void wifiTestButtonEvent(lv_event_t * e)
{
	xTaskCreate(wifiTestTask, "testWifi", 20000, NULL, tskIDLE_PRIORITY, NULL);
}
void wifiTestTask(void *pvParameters)
{
    char ssidCharArray[64];
    lv_dropdown_get_selected_str(ui_wifiChooserDropdown,ssidCharArray,sizeof(ssidCharArray));
    strncpy(WIFI_SSID,ssidCharArray,sizeof(WIFI_SSID));
    char passwordText[64];
    memcpy(passwordText,lv_textarea_get_text(ui_passwordTextArea),sizeof(passwordText));
    memcpy(WIFI_PASSWORD,passwordText,sizeof(WIFI_PASSWORD));
    WiFi.mode(WIFI_STA);
    char wifiTestLabelString[64];
    char clearWifiString[64];
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    Serial.print("Trying to connect to: ");
    Serial.print(WIFI_SSID);
    strcat(wifiTestLabelString, "Trying to connect to: ");
    strcat(wifiTestLabelString, WIFI_SSID);
    memcpy(wifiTestLabel,wifiTestLabelString,sizeof(wifiTestLabel));
    int c = 0;
    while(WiFi.status() != WL_CONNECTED and c < 20)
    {
        Serial.print(".");
        if( c%5 == 0)
        {
            memcpy(wifiTestLabelString,clearWifiString,sizeof(wifiTestLabelString));
            strcat(wifiTestLabelString,"Trying to connect to: ");
            strcat(wifiTestLabelString,WIFI_SSID);
        }
        else
        {
            strcat(wifiTestLabelString,".");
        }
        memcpy(wifiTestLabel,wifiTestLabelString,sizeof(wifiTestLabel));
        c++;
        delay(1000);
    }
    Serial.println("");
    memcpy(wifiTestLabelString,clearWifiString,sizeof(wifiTestLabelString));
    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.print("Connection success, closing connection");
        strcat(wifiTestLabelString,"Connection succesful, WIFI configuration saved.");
    }
    else
    {
        strcat(wifiTestLabelString,"Connection unsuccessful turning off wifi.");
    }
    Serial.println(wifiTestLabelString);
    memcpy(wifiTestLabel,wifiTestLabelString,sizeof(wifiTestLabel));
    delay(3000);
    closeWifiServer();
    _ui_screen_change(&ui_EntranceScreen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_EntranceScreen_screen_init);
    vTaskDelete(NULL);
}

void resetDeviceButtonEvent(lv_event_t * e)
{
	resetDevice();
}
void resetDevice()
{
    esp_restart();
}

void screenOffButtonEvent(lv_event_t * e)
{
	screenOff();
}
void screenOff()
{

}

void startBluetoothButtonEvent(lv_event_t * e)
{
	startBluetooth();
}
void startBluetooth()
{
	// Your code here
}

void closeBluetoothButtonEvent(lv_event_t * e)
{
	closeBluetooth();
}
void closeBluetooth()
{
	// Your code here
}
