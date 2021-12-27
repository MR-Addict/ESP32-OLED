void SetupInit() {
  Serial.begin(115200);

  DataLength = myData.GetDataLength();
  Wire.begin();

  rtc.begin();
  DateTime now = rtc.now();
  YEAR = now.year(), MONTH = now.month(), DATE = now.day();
  HOUR = now.hour(), MINUTE = now.minute(), SECOND = now.second();

  bme.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);

}

void UpdateData() {
  EVERY_N_SECONDS(1) {
    DateTime now = rtc.now();
    YEAR = now.year(), MONTH = now.month(), DATE = now.day();
    HOUR = now.hour(), MINUTE = now.minute(), SECOND = now.second(), DayOfTheWeek = now.dayOfTheWeek();

    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);

    String TIME = (String)YEAR + '-' + (String)MONTH + '-' + (String)DATE + ' ' + (String)HOUR + ':' + (String)MINUTE + ':' + (String)SECOND;
    ReturnData.temperature = temp, ReturnData.pressure = pres, ReturnData.time = TIME;
  }
}

void DisplayTime() {
  static boolean flag;
  String TimeData;
  EVERY_N_SECONDS(1) {
    flag = !flag;
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
    display.print(daysOfTheWeek[DayOfTheWeek]);
    display.display();
  }
}

void DisplayTemp() {
  EVERY_N_SECONDS(1) {
    String TempData;
    if (ReturnData.temperature < 0)TempData = '-';
    else if (ReturnData.temperature < 10)TempData = '0';
    TempData += (uint8_t)ReturnData.temperature;

    display.clearDisplay();
    display.drawBitmap(0, 1, Temperature, 32, 32, WHITE);
    display.setCursor(33, 32);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.print(TempData);
    display.print('.');
    display.setFont(&DSEG7_Classic_Bold_15);
    display.print(((uint8_t)(ReturnData.temperature * 100)) % 100 < 10 ? '0' + (String)(((uint8_t)(ReturnData.temperature * 100)) % 100) : ((uint8_t)(ReturnData.temperature * 100)) % 100);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.print("-C");
    display.display();
  }
}

void DisplayPressure() {
  EVERY_N_SECONDS(1) {
    display.clearDisplay();
    display.drawBitmap(0, 1, Pressure, 40, 32, WHITE);
    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(28, 31);
    display.print(((uint32_t)ReturnData.pressure) / 100000);
    display.print('.');
    display.setFont(&DSEG7_Classic_Bold_15);
    display.print((((uint32_t)ReturnData.pressure) % 100000) / 10000);
    display.print((((uint32_t)ReturnData.pressure) % 10000) / 1000);
    display.print((((uint32_t)ReturnData.pressure) % 1000) / 100);
    display.setFont(&FreeMono9pt7b);
    display.print("atm");
    display.display();
  }
}

void SendData() {
  EVERY_N_HOURS(1) {
    String message = "{\"temperature\":" + (String)ReturnData.temperature + ",\"pressure\":" + (String)ReturnData.pressure +
                     ",\"time\":\"" + (String)ReturnData.time + "\"}";
    websocket.broadcastTXT(message);
    myData.PushData(ReturnData.temperature, ReturnData.pressure, ReturnData.time);
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

void OLED() {
  if (isDisplay) {
    switch (Mode) {
      case 0: DisplayTime(); break;
      case 1: DisplayTemp(); break;
      case 2: DisplayPressure(); break;
      case 3: DisplayIP(); break;
    }
  }
}
