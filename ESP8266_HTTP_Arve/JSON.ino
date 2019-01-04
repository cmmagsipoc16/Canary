
String Serialize()
{
  // Allocate the JSON document
  StaticJsonBuffer<256> data;
  // Make our document as an object
  JsonObject& object = data.createObject(); //300 bytes

  object["temp"] = t;
  object["humidity"] = h;
  object["voc"] = VOC;
  object["co_2"] = CO2;
  object["pm_25"] = PM2_5;
  object["int_aqi"] = aqi;
  object["int_color"] = aqiColor;
  object["category"] = aqiCategory;

//Display to Serial using either:
//  object.printTo(Serial);
//  Serial.println();

  object.prettyPrintTo(Serial);
  //Serial.println();

  char jsonChar[256];
  object.printTo((char*)jsonChar, object.measureLength()+1);
  data_json = String(jsonChar);
  //Serial.print(data_json);
  
  return data_json;
}
