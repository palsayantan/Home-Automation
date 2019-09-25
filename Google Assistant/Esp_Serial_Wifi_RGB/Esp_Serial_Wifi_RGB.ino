
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>

String ID = "";            //Enter Product ID
int req;                  //Handle Web Request

#define PIN        5         //RGB LED Pin
#define NUMPIXELS  1        //No. of LEDs
#define TIMEOUT    90      //AP timeout in sec

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(150, 0, 150));
  pixels.show();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void wifi() {
  pixels.clear();
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    pixels.setPixelColor(0, pixels.Color(150, 150, 150));
    pixels.show();
    delay(500);
    pixels.clear();
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
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(0, 0, 150));
    pixels.show();
    int httpCode = http.GET();
    String response = "";
    if (httpCode > 0) {
      // start of fetching get process
      response = http.getString();
      Serial.println(response);
      req = response.toInt();
      Serial.write(req);
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      pixels.show();
    }
    else {
      Serial.println(httpCode);
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
      pixels.show();
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
