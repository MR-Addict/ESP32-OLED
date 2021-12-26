void SetupInit() {
  Serial.begin(115200);

  EEPROM.begin(28 * 24);
  DataLength = myData.GetDataLength();
  Wire.begin();

  rtc.begin();
  DateTime now = rtc.now();
  YEAR = now.year(), MONTH = now.month(), DATE = now.day();
  HOUR = now.hour(), MINUTE = now.minute(), SECOND = now.second();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.ssd1306_command(SSD1306_DISPLAYOFF);

  bme.begin();
}

void DisplayTime() {
  static boolean flag;
  String TimeData;
  EVERY_N_HOURS(1) {
    flag = !flag;
    DateTime now = rtc.now();
    YEAR = now.year(), MONTH = now.month(), DATE = now.day();
    HOUR = now.hour(), MINUTE = now.minute(), SECOND = now.second();

    if (HOUR < 10)TimeData += '0' + (String)HOUR;
    else TimeData += (String)HOUR;
    if (flag)TimeData += ':';
    else TimeData += ' ';
    if (MINUTE < 10)TimeData += '0' + (String)MINUTE;
    else TimeData += (String)MINUTE;

    display.clearDisplay();
    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(0, 32);
    display.print(TimeData);

    display.setFont(&FreeMono9pt7b);
    display.setCursor(92, 31);
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);
    display.display();
  }
}

void DisplayTemp() {
  EVERY_N_SECONDS(1) {
    String TempData;
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);

    if (temp < 0)TempData = '-';
    else if (temp < 10)TempData = '0';
    TempData += (uint8_t)temp;

    display.clearDisplay();
    display.drawBitmap(0, 1, Temperature, 32, 32, WHITE);
    display.setCursor(33, 32);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.print(TempData);
    display.print('.');
    display.setFont(&DSEG7_Classic_Bold_15);
    display.print(((uint8_t)(temp * 100)) % 100 < 10 ? '0' + (String)(((uint8_t)(temp * 100)) % 100) : ((uint8_t)(temp * 100)) % 100);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.print("-C");
    display.display();
  }
}

void DisplayPressure() {
  EVERY_N_SECONDS(1) {
    String TempData;
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);

    display.clearDisplay();
    display.drawBitmap(0, 1, Pressure, 40, 32, WHITE);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(28, 31);
    display.print(((uint32_t)pres) / 100000);
    display.print('.');
    display.setFont(&DSEG7_Classic_Bold_15);
    display.print((((uint32_t)pres) % 100000) / 10000);
    display.print((((uint32_t)pres) % 10000) / 1000);
    display.print((((uint32_t)pres) % 1000) / 100);
    display.setFont(&FreeMono9pt7b);
    display.print("atm");
    display.display();
  }
}

void SendData() {
  EVERY_N_HOURS(1) {
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);
    String message = "{\"temperature\":" + (String)temp + ",\"pressure\":" + (String)pres +
                     ",\"time\":\"" + YEAR + '-' + MONTH + '-' + DATE + ' ' +
                     HOUR + ':' + MINUTE + ':' + SECOND + "\"}";
    websocket.broadcastTXT(message);
    String TIME = (String)YEAR + '-' + (String)MONTH + '-' + (String)DATE + ' ' + (String)HOUR + ':' + (String)MINUTE + ':' + (String)SECOND;
    myData.PushData(temp, hum, TIME);
    DataLength = myData.GetDataLength();
  }
}

void DisplayIP() {
  EVERY_N_SECONDS(1) {
    display.clearDisplay();
    display.setFont(&FreeMono9pt7b);
    display.setCursor(0, 10);
    display.print(WiFi.localIP());
    display.display();
  }
}

void GetButtons() {
  if (Button1.isPressed()) {
    isDisplay = !isDisplay;
    if (isDisplay)display.ssd1306_command(SSD1306_DISPLAYON);
    else display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
  if (isDisplay) {
    if (Button2.isPressed() && Mode > 0)Mode--;
    else if (Button3.isPressed() && Mode < 3)Mode ++;
  }
}
