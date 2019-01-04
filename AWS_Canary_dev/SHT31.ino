#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool initSHT31() {
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1);
    return false;
  }
  return true;
}

float readTempSHT31(){
  float t = sht31.readTemperature();
  if (! isnan(t)) {  // check if 'is not a number'
  //  Serial.print("Temp = "); Serial.print(t); Serial.println(" *C");
  } else { 
    Serial.println("Failed to read temperature");
  }
  return t;
}

float readHumiditySHT31(){  
  float h = sht31.readHumidity();
  if (! isnan(h)) {  // check if 'is not a number'
  //  Serial.print("Hum. = "); Serial.print(h); Serial.println(" %RH");
  } else { 
    Serial.println("Failed to read humidity");
  }
  return h;
}
