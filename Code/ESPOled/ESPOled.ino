#include <WiFi.h>
#include <Wire.h>
#include <FastLED.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>

#include <RTClib.h>
#include <BME280I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/DSEG7_Classic_Bold_25.h>
#include <Fonts/DSEG7_Classic_Bold_15.h>
#include <Fonts/FreeMono9pt7b.h>

#include "MyData.h"
#include "Button.h"
#include "Header.h"
#include "Web.h"
#include "Basic.h"

void setup() {
  SetupInit();
  WIFIInit();
  ServerInit();
}

void loop() {
  GetButtons();
  UpdateData();
  SendData();
  GetButtons();
  OLED();
  websocket.loop();
}
