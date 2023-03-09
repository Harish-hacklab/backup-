#include <WiFi.h>
//#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include <SaffrAsyncOTA.h>
const char* ssid = "HACKLAB@2G";
const char* password = "HACK@LAB";
String streeng;

AsyncWebServer server(80);
bool readFile(fs::FS & fs, const char * path);


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("updated");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (!SPIFFS.begin(true)) {
    //Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  if (SPIFFS.exists("/config.txt"))
  {
    Serial.println("File received");  
    readFile(SPIFFS, "/config.txt");
  }
  SaffrAsyncOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  SaffrAsyncOTA.loop();
}

bool readFile(fs::FS & fs, const char * path)
{
  //  Software.printf("Opened to read the file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println(F("- failed to open file for reading"));
    return false;
  }

  Serial.print(F("Reading :"));
  if (file.available())
  {
    streeng = file.readString();
  }

  Serial.println(F("Complete"));
  Serial.println(F("In reading fn : "));  Serial.println(streeng);
  vTaskDelay(3);
  return true;
}
