#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;

int counter = 0;
/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

bool initSGP30() 
{  
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1); 
    return false;
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
  return true;
}


//void readSGP30 (int* VOC, int* CO2){
void readSGP30_Baseline (){
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  //Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.println(" ppb\t");
  //Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  //counter++;
  //if (counter == 30) {
  //  counter = 0;

  //  uint16_t TVOC_base, eCO2_base;
   // if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
  //    Serial.println("Failed to get baseline readings");
  //    return;
  //  }
  //  Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
  //  Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  //}
}

int readCO2SGP30(){
  return sgp.eCO2;
  }

int readTVOCSGP30(){
  return sgp.TVOC;
  }
