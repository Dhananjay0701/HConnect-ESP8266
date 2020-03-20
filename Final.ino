/////// Header Files

// // IRsend
// #ifndef UNIT_TEST
// #include <Arduino.h>
// #endif
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include <ir_Panasonic.h>

// WiFiManager
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

// Google Asst
#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>

// OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// /////// Global Variables

// // IRsend
// const uint16_t kIrLed = 4;
// IRPanasonicAc ac(kIrLed);

// Google Asst
const char* site = ""; // HConnect API Link
const char* getRemoteStatusSite = "/read/RemoteStatus";
const char* setRemoteStatusSite = "/write/RemoteStatus/0";
const char* getACStatusSite = "/read/ACStatus";
const char* getACTempStatusSite = "/read/ACTemp";
const char* getRelayStatusSite = "/read/RelayStatus";
String chipID;

const uint16_t relay1 = 14;
const uint16_t relay2 = 12;
const uint16_t relay3 = 13;
const uint16_t relay4 = 5;

// Delay - ESP Reset
const uint16_t delayLimit = 10;
uint16_t delayCurr = 0;

// OTA SERVER
bool OTAFlag = false;
ESP8266WebServer serverOTA;


/////// Setup Functions

void wifiManagerSetup() {
  WiFiManager wifiManager;  
  wifiManager.autoConnect();
}

// void irSendSetup() {
//   ac.begin();
// }

void relaySetup(){
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);

  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);

  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, HIGH);

  pinMode(relay4, OUTPUT);
  digitalWrite(relay4, HIGH);
}

void otaSetup(){
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");

    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");

    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");

    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");

    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  serverOTA.on("/OTA", [](){
    OTAFlag = true;
    Serial.println("OTA Time !!!!!!!!!!");
    serverOTA.send(200, "text/plain", "OTA Time !!!!!!!!!!");
  });
  serverOTA.begin();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // irSendSetup();
  wifiManagerSetup();
  relaySetup();
  otaSetup();

  chipID = String(ESP.getChipId());
  Serial.println(chipID);
}

////// Helper Functions

// void printState() {
//   Display the settings.
//   Serial.println("Panasonic A/C remote is in the following state:");
//   Serial.printf("  %s\n", ac.toString().c_str());

//   Display the encoded IR sequence.
//   unsigned char* ir_code = ac.getRaw();
//   Serial.print("IR Code: 0x");

//   for (uint8_t i = 0; i < kPanasonicAcStateLength; i++)
//     Serial.printf("%02X", ir_code[i]);
//   Serial.println();
// }

// void ACOn(int temp) {
//   //Serial.println("Default state of the remote.");
//   //printState();
//   //Serial.println("Setting desired state for A/C.");
//   ac.setModel(kPanasonicRkr);
//   ac.on();
//   ac.setFan(kPanasonicAcFanMax);
//   ac.setMode(kPanasonicAcCool);
//   ac.setTemp(temp);
//   ac.setSwingVertical(kPanasonicAcSwingVAuto);
//   ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
//   #if SEND_PANASONIC_AC
//     //Serial.println("Sending IR command to A/C ...");
//     ac.send();
//   #endif  // SEND_PANASONIC_AC
//     //printState();
// }

// void ACOff(int temp) {
//   //Serial.println("Default state of the remote.");
//   //printState();
//   //Serial.println("Setting desired state for A/C.");
//   ac.setModel(kPanasonicRkr);
//   ac.off();
//   ac.setFan(kPanasonicAcFanMax);
//   ac.setMode(kPanasonicAcCool);
//   ac.setTemp(temp);
//   ac.setSwingVertical(kPanasonicAcSwingVAuto);
//   ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
//   #if SEND_PANASONIC_AC
//     //Serial.println("Sending IR command to A/C ...");
//     ac.send();
//   #endif  // SEND_PANASONIC_AC
//     //printState();
// }

// uint16_t getTemp() {
//   uint16_t temp = 16; 
//   HTTPClient http;

//   http.begin(site + chipID + getACTempStatusSite);
//   uint16_t httpCode = http.GET();

//   if(httpCode > 0){           
//     temp = http.getString().toInt();     
//   }

//   http.end();
//   return temp;
// }

// bool getPow() {
//   uint16_t pow = 1;    
//   HTTPClient http;

//   http.begin(site + chipID + getACStatusSite);
//   uint16_t httpCode = http.GET();

//   if(httpCode > 0){
//     pow = http.getString().toInt();     
//   }  

//   http.end();
//   if(pow == 0)
//     return false;
//   return true;
// }

// void updateStatus(){   
//   HTTPClient http;

//   http.begin(site + chipID + setRemoteStatusSite);
//   uint16_t httpCode = http.GET();
//   //Serial.println("Updated String http Code ..." + String(httpCode));

//   http.end();
// }

void relayCheck(){   
  HTTPClient http;
    
  http.begin(site + chipID + getRelayStatusSite);
  uint16_t httpCode = http.GET();
    
  if(httpCode > 0){
    int relayStat = http.getString().toInt();
    Serial.println(relayStat);

    if(relayStat % 10 == 1)
      Serial.println("Relay 1 ... ON");
    else
      Serial.println("Relay 1 ... OFF");
    
    if(relayStat % 10 == 1)
      digitalWrite(relay1, LOW);
    else
      digitalWrite(relay1, HIGH);
    relayStat /= 10;


    if(relayStat % 10 == 1)
      Serial.println("Relay 2 ... ON");
    else
      Serial.println("Relay 2 ... OFF");

    if(relayStat % 10 == 1)
      digitalWrite(relay2, LOW);
    else
      digitalWrite(relay2, HIGH);
    relayStat /= 10;


    if(relayStat % 10 == 1)
      Serial.println("Relay 3 ... ON");
    else
      Serial.println("Relay 3 ... OFF");

    if(relayStat % 10 == 1)
      digitalWrite(relay3, LOW);
    else
      digitalWrite(relay3, HIGH);
    relayStat /= 10;


    if(relayStat % 10 == 1)
      Serial.println("Relay 4 ... ON");
    else
      Serial.println("Relay 4 ... OFF");

    if(relayStat % 10 == 1)
      digitalWrite(relay4, LOW);
    else
      digitalWrite(relay4, HIGH);
    relayStat /= 10;
  }
    
  http.end();
  delay(100);
}


// void ACRemoteCheck() { 
//   HTTPClient http;

//   http.begin(site + chipID + getRemoteStatusSite);
//   uint16_t httpCode = http.GET();

//   if(httpCode > 0){
//     uint16_t remoteStat = http.getString().toInt();

//     if(remoteStat == 1){
//       digitalWrite(LED_BUILTIN, LOW);
//       uint16_t temp = getTemp();

//       if(getPow())
//         ACOn(temp);
//       else
//         ACOff(temp);

//         //Serial.println("Updated String ..." + String(ans));
//         updateStatus();
//         digitalWrite(LED_BUILTIN, HIGH);
//     }
//   }         
//   http.end();
//   delay(100);
// }

void otaHelper(){
  serverOTA.handleClient();
  if(OTAFlag){
    int OTATimeElapsed;
    int OTATimeStart;
    OTATimeStart = millis();
    while(OTATimeElapsed - OTATimeStart < 30000){
      Serial.println("OTA WAIT :(");
      ArduinoOTA.handle();
      OTATimeElapsed = millis();
      delay(10);
    }
    OTAFlag = false;
  }
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    relayCheck();
    // ACRemoteCheck();
  }
  else {
    if(delayCurr < delayLimit){
      delay(1000);
      delayCurr++;
    }
    else
      ESP.reset();
  }
  otaHelper();
}
