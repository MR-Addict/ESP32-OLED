struct WeathData {
  float temperature;
  float pressure;
  String time;
};

class cData {
  private:
    uint8_t DataSize;
    uint8_t DataLength;
    uint8_t MaxLength;
    WeathData _Data;
    void WriteData(uint8_t address, float Temp, float Pressure, String Date);
  public:
    cData() {
      DataSize = 28;
      DataLength = EEPROM.read(0);
      MaxLength = 24;
    }
    WeathData ReadData(uint16_t address);
    void PushData(float Temp, float Pressure, String Date);
    void SetDataSize(uint8_t dataSize);
    void SetMaxLength(uint8_t maxLength);
    uint8_t GetDataLength();
};

void cData::SetDataSize(uint8_t dataSize) {
  DataSize = dataSize;
}
void cData::SetMaxLength(uint8_t maxLength) {
  MaxLength = maxLength;
}

uint8_t cData::GetDataLength() {
  return DataLength;
}

void cData::WriteData(uint8_t address, float Temp, float Pressure, String Date) {
  EEPROM.writeFloat(address * DataSize + 1, Temp);
  EEPROM.writeFloat(address * DataSize + 5, Pressure);
  EEPROM.writeString(address * DataSize + 9, Date);
  if (DataLength < MaxLength) {
    DataLength++;
    EEPROM.write(0, DataLength);
  }
}


WeathData cData::ReadData(uint16_t address) {
  _Data.temperature = EEPROM.readFloat(address * DataSize + 1);
  _Data.pressure = EEPROM.readFloat(address * DataSize + 5);
  _Data.time = EEPROM.readString(address * DataSize + 9);
  return _Data;
}

void cData::PushData(float Temp, float Pressure, String Date) {
  if (DataLength < MaxLength)WriteData(DataLength, Temp, Pressure, Date);
  else {
    for (uint8_t i = 1; i < MaxLength; i++) {
      ReadData(i);
      WriteData(i - 1, _Data.temperature, _Data.pressure, _Data.time);
    }
    WriteData(MaxLength - 1, Temp, Pressure, Date);
  }
}
