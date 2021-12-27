struct WeathData {
  float temperature;
  float pressure;
  String time;
};

class cData {
  private:
    uint8_t DataLength;
    uint8_t MaxLength;
    WeathData *_Data;
    void WriteData(uint8_t address, float Temp, float Pressure, String Date);
  public:
    cData(WeathData *DataAddress, uint8_t maxLength) {
      _Data = DataAddress;
      MaxLength = maxLength;
    }
    WeathData ReadData(uint8_t address);
    void PushData(float Temp, float Pressure, String Date);
    uint8_t GetDataLength();
};

uint8_t cData::GetDataLength() {
  return DataLength;
}

void cData::WriteData(uint8_t address, float Temp, float Pressure, String Date) {
  _Data[address].temperature = Temp;
  _Data[address].pressure = Pressure;
  _Data[address].time = Date;
  if (DataLength < MaxLength) {
    DataLength++;
  }
}

WeathData cData::ReadData(uint8_t address) {
  return _Data[address];
}

void cData::PushData(float Temp, float Pressure, String Date) {
  if (DataLength < MaxLength)WriteData(DataLength, Temp, Pressure, Date);
  else {
    for (uint8_t i = 1; i < MaxLength; i++)_Data[i - 1] = _Data[i];
    WriteData(MaxLength - 1, Temp, Pressure, Date);
  }
}
