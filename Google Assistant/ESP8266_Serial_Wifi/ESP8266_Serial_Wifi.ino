//need some kind of ping method so that our app know when the device gone offline

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

String ID=""; //Enter Product ID

#define TIMEOUT 90        //AP timeout in sec
int req;                  //Handle Web Request

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;                                                  //WiFiManager
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  //wifiManager.resetSettings();                                            //reset saved settings
  wifiManager.setTimeout(TIMEOUT);                                          //sets timeout until configuration portal gets turned off in seconds
  //wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));   //set custom ip for portal
  if (!wifiManager.autoConnect("Wifi Manager", "EspConfig")) {    //fetches ssid and pass from eeprom and tries to connect
    Serial.println("failed to connect and hit timeout");                    //if it does not connect it starts an access point with the specified name
    delay(1000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
  }
  Serial.println("Connected to Wifi ");                                     //if you get here you have connected to the WiFi
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//  link for ESP to hit
//  https://customprojects.000webhostapp.com/Home_Automation/[ID]/state

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void wifi() {
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting..");
  }
  action();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String link = ("http://customprojects.000webhostapp.com/Home_Automation/");
    String url = (link + ID + "/state");
    //Serial.println(url);
    http.begin(url);
    int httpCode = http.GET();
    String response = "";
    if (httpCode > 0) {
      // start of fetching get process
      response = http.getString();
      Serial.println(response);
      req=response.toInt();
      Serial.write(req);
    }
    else {
      Serial.println(httpCode);
      delay(1000);
      action();
    }
    http.end();
    delay(500);
  }
}

void loop() {
  wifi();
}
