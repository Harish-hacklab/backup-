#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "esp_system.h"
#include <Adafruit_Fingerprint.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include "FS.h"
#include <ArduinoJson.h>



String employeeName;
String oldname;
int operatorID = -1;
String streeng;
DynamicJsonBuffer jsonBuffer(200); // look into the size
hw_timer_t *timer = NULL;
SoftwareSerial Software(5, 23, false, 256);
const int wdtTimeout = 15000;  //time in ms to trigger the watchdog
void updateCheck();
void setupWatchdog(const int wdtTimeout);
String str;
uint32_t looptime = 0;
HardwareSerial FingerPrint(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerPrint); //white - 27.....yellow - 26
bool fp_flag = false;
byte fingerp[534];
int id = 0;

void setupFiles();
void IRAM_ATTR resetModule();
void setupFingerPrint();
bool checkFingerPrint();
uint8_t deleteFingerprint(int id);
uint8_t getFingerprintEnroll(int enroll_ID);
bool enroll(int enroll_ID);


WebServer server(80);
//long int enrol_time = millis() - 180000;
//bool enrol_flag = false;
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, String message);
void handleList();


bool is_authentified();
void handleLogin();
void handleOption();
void handleUser();
void handleDelete();
void handleRemoval();
//void handlef1();
//void handlef2();
//void handlef3();
//void handlef4();
void handleRoot();
void handleNotFound();
void setupServer();
