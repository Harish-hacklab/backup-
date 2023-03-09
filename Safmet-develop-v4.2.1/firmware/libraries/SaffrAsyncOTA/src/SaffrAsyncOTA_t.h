#ifndef SaffrAsyncOTA_h
#define SaffrAsyncOTA_h

#include "Arduino.h"
#include "SPIFFS.h"
#include "stdlib_noniso.h"

#if defined(ESP8266)
    #include "ESP8266WiFi.h"
    #include "ESPAsyncTCP.h"
    #include "flash_hal.h"
    #include "FS.h"
#elif defined(ESP32)
    #include "WiFi.h"
    #include "AsyncTCP.h"
    #include "Update.h"
    #include "esp_int_wdt.h"
    #include "esp_task_wdt.h"
#endif

#include "ESPAsyncWebServer.h"


class SaffrAsyncOTAClass{

    public:

        void setID(const char* id){
            _id = id;
        }

        void begin(AsyncWebServer *server, const char* username = "", const char* password = ""){
            _server = server;

            if(strlen(username) > 0){
                _authRequired = true;
                _username = username;
                _password = password;
            }else{
                _authRequired = false;
                _username = "";
                _password = "";
            }
			_server->on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
				Serial.println("getting index request");
				request->send(SPIFFS, "/index.html", String(), false);
		});
			_server->on("/hacklab.png", HTTP_GET, [](AsyncWebServerRequest * request) {
				request->send(SPIFFS, "/hacklab.png", "text/img");
		});
			_server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
				request->send(SPIFFS, "/style.css", "text/css");
		});
			_server->on("/serverIndex.html", HTTP_GET, [](AsyncWebServerRequest * request) {
				request->send(SPIFFS, "/serverIndex.html", "text/html");
		});
			_server->on("/status", HTTP_GET, [](AsyncWebServerRequest * request) {
				Serial.println("received request");
				File download = SPIFFS.open("/config.txt",  "r");
				//response->close();
		});

            _server->on("/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
                // the request handler is triggered after the upload has finished... 
                // create the response, add header, and send response
                AsyncWebServerResponse *response = request->beginResponse((Update.hasError())?500:200, "text/plain", (Update.hasError())?"FAIL":"OK");
                response->addHeader("Connection", "close");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                restartRequired = true;
            }, [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                //Upload handler chunks in data
				File file; 
				bool spiffs = false;
                if (!index) {
                    #if defined(ESP8266)
                        int cmd = (filename == "filesystem") ? U_FS : U_FLASH;
                        Update.runAsync(true);
                        size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
                        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                        if (!Update.begin((cmd == U_FS)?fsSize:maxSketchSpace, cmd)){ // Start with max available size
                    #elif defined(ESP32)
					Serial.print("/nFILE NAME : ");Serial.println(filename);
					int cmd;
					if(filename.endsWith(".bin"))
					{
					Serial.println("Bin file");
					cmd = U_FLASH;
					}
					else
					{
						Serial.println("Spiffs file received");
						cmd = U_SPIFFS;
						spiffs = true;
						filename = "/" + filename;
						SPIFFS.remove(filename);
						file = SPIFFS.open(filename, "w");
					}
                        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) { // Start with max available size
                    #endif
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                // Write chunked data to the free sketch space
                if(len){
					Serial.print(".");//Serial.println(len);
					//Serial.print("final : ");//Serial.println(final);
					if(spiffs)
					{
						file.write(data,len);
					}
                    else if (Update.write(data, len) != len) {
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }
                    
                if (final) { // if the final flag is set then this is the last frame of data
					Serial.println("\nCode Uploaded Successfully!!!");
                    if (!Update.end(true)) { //true to set the size to the current progress
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "Could not end OTA");
                    }
                }else{
					if(spiffs)
					{
						file.close();
					}
                    return;
                }
            });
        }

        void loop(){
            if(restartRequired){
                yield();
                delay(1000);
                yield();
                #if defined(ESP8266)
                    ESP.restart();
                #elif defined(ESP32)
                    // ESP32 will commit sucide
					esp_restart();
                    //esp_task_wdt_init(1,true);
                    //esp_task_wdt_add(NULL);
                    while(true);
                #endif
            }
        }

    private:
        AsyncWebServer *_server;

        String getID(){
            String id = "";
            #if defined(ESP8266)
                id = String(ESP.getChipId());
            #elif defined(ESP32)
                id = String((uint32_t)ESP.getEfuseMac(), HEX);
            #endif
            id.toUpperCase();
            return id;
        }

        String _id = getID();
        String _username = "";
        String _password = "";
        bool _authRequired = false;
        bool restartRequired = false;

};

SaffrAsyncOTAClass SaffrAsyncOTA;
#endif
