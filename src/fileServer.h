#include <WiFi.h>              // Built-in
#include <WebServer.h>
#include "SPIFFS.h"
#include "FS.h"
#define ServerVersion ""
String  webpage = "";
#include "CSS.h"

bool    SPIFFS_present = false;

WebServer server(80);
//Forward Declarations
String file_size(int bytes);
String tempValString = "";
String phValString = "";
void HomePage();
void DownloadFile(String filename);
void SPIFFS_file_delete(String filename);
void SendHTML_Header();
void SendHTML_Content();
void SendHTML_Stop();
void ReportSPIFFSNotPresent();
void ReportFileNotPresent(String target);
void live_view();
void File_Download_PH_LOG();
void File_Download_TEMP_LOG();
void Delete_Log_Files();
String file_size(int bytes);

String serverSetup(String ssid, String password,bool isAP){
  String retIpAddress;
  if (isAP)
  {
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED) 
    { // Wait for the Wi-Fi to connect
      delay(250); Serial.print('.');
    }
    Serial.println("\nConnected to "+WiFi.SSID()+" Use IP address: "+WiFi.localIP().toString()); // Report which SSID and IP is in use
    retIpAddress = WiFi.localIP().toString();
  }
  else
  {
    WiFi.softAP("PH Sensor");
    retIpAddress = WiFi.softAPIP().toString();
  }
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialisation failed...");
    SPIFFS_present = false; 
  }
  else
  {
    Serial.println(F("SPIFFS initialised... file access enabled..."));
    SPIFFS_present = true; 
  }
  ///////////////////////////// Server Commands 
  server.on("/",         HomePage);
  server.on("/liveview", live_view);
  server.on("/download_ph_log", File_Download_PH_LOG);
  server.on("/download_temp_log", File_Download_TEMP_LOG);
  server.on("/delete_log_files",   Delete_Log_Files);
  
  ///////////////////////////// End of Request commands
  server.begin();
  Serial.println("HTTP server started");
  return retIpAddress;
}
void serverLoop(float tempVal,float phVal){
  tempValString = String(tempVal);
  phValString = String(phVal);
  server.handleClient(); // Listen for client connections
}
void HomePage(){
  SendHTML_Header();
  webpage += F("<a href='/liveview'><button>PH Sensor Live View</button></a>");
  webpage += F("<a href='/download_ph_log'><button>Download PH Log</button></a>");
  webpage += F("<a href='/download_temp_log'><button>Download Temp Log</button></a>");
  webpage += F("<a href='/delete_log_files'><button>Delete Log Files</button></a>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop(); // Stop is needed because no content length was sent
}
void File_Download_PH_LOG()
{
  DownloadFile("PHLog.txt");
}
void File_Download_TEMP_LOG()
{
  DownloadFile("TEMPLog.txt");
}
void live_view()
{
  SendHTML_Header();
  webpage += ("<meta http-equiv=\"refresh\" content=\"5\">");
  webpage += ("<button disabled>PH Value: "+phValString+"</button>");
  webpage += ("<button disabled>T  Value: "+tempValString+"</button>");

  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
void DownloadFile(String filename){
  if (SPIFFS_present) { 
    File download = SPIFFS.open("/"+filename,  "r");
    if (download) {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    } else ReportFileNotPresent("download"); 
  } else ReportSPIFFSNotPresent();
}
void Delete_Log_Files(){
  SPIFFS_file_delete("TEMPLog.txt");
  SPIFFS_file_delete("PHLog.txt");
}
void SPIFFS_file_delete(String filename) { // Delete the file 
  if (SPIFFS_present) { 
    SendHTML_Header();
    File dataFile = SPIFFS.open("/"+filename, "r"); // Now read data from SPIFFS Card 
    if (dataFile)
    {
      if (SPIFFS.remove("/"+filename)) {
        Serial.println(F("File deleted successfully"));
        webpage += "<h3>Log Files Have Been Deleted</h3>"; 
        webpage += F("<a href='/'>[Back]</a><br><br>");
      }
      else
      { 
        webpage += F("<h3>File was not deleted - error</h3>");
        webpage += F("<a href='/'>[Back]</a><br><br>");
      }
    } else ReportFileNotPresent("delete");
    append_page_footer(); 
    SendHTML_Content();
    SendHTML_Stop();
  } else ReportSPIFFSNotPresent();
} 
void SendHTML_Header(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1"); 
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves. 
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}
void SendHTML_Content(){
  server.sendContent(webpage);
  webpage = "";
}
void SendHTML_Stop(){
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}
void ReportSPIFFSNotPresent(){
  SendHTML_Header();
  webpage += F("<h3>No SPIFFS Card present</h3>"); 
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
void ReportFileNotPresent(String target){
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
void ReportCouldNotCreateFile(String target){
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}
