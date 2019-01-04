float computeAQI (float PM2_5Value){
  float aqiValue;

  if (PM2_5Value >= 0.0 && PM2_5Value <= 12.0)  {
    aqiValue = map(PM2_5Value, 0.0, 12.0, 0, 50);
  }
  else if (PM2_5Value >= 12.1 && PM2_5Value <= 35.4) {
    aqiValue = map(PM2_5Value, 12.1, 35.4, 50.1, 100);
  }
  else if (PM2_5Value >= 35.5 && PM2_5Value <= 55.4) {
    aqiValue = map(PM2_5Value, 35.5, 55.4, 100.1, 150) ;
  }
  else if (PM2_5Value >= 55.5 && PM2_5Value <= 150.4){
    aqiValue = map(PM2_5Value, 55.5, 150.4, 150.1, 200);
  }
  else if (PM2_5Value >= 150.5 && PM2_5Value <= 250.4){
    aqiValue = map(PM2_5Value, 150.5, 250.4, 200.1, 300);
  }
  else if (PM2_5Value >= 250.5 && PM2_5Value <= 350.4) {
    aqiValue = map(PM2_5Value, 250.5, 350.4, 300.1, 400);
  }
  else if (PM2_5Value >= 350.5) {
    aqiValue = map(PM2_5Value, 350.5, 500, 400.1, 500);
  }
  
  return aqiValue;
}

String setAQIColor (float AQI){
  String color;

  if (AQI >= 0.0 && AQI <= 50.0) {
    color = "Green";
  }
  else if (AQI >= 50.1 && AQI <= 100){
    color = "Yellow";
  }
  else if (AQI >= 100.1 && AQI <= 150.0){
    color = "Orange";
  }
  else if (AQI >= 150.1 && AQI <= 200.0){
    color = "Red";
  }
  else if (AQI >= 200.1 && AQI <= 300.0){
    color = "Purple";
  }
  else if (AQI >= 300.1){
    color = "Maroon";
  }

/////////////////////////////////  writeAqiLight(AQI);

  return color;
}

String setAQICategory (float AQI){
  String category;

  if (AQI >= 0.0 && AQI <= 50.0){
    category = "Healthy";
  }
  else if (AQI >= 50.1 && AQI <= 100){
    category = "Moderate";
  }
  else if (AQI >= 100.1 && AQI <= 150.0){
    category = "Unhealthy_for_Sensitive_Groups";
  }
  else if (AQI >= 150.1 && AQI <= 200.0) {
    category = "Unhealthy";
  }
  else if (AQI >= 200.1 && AQI <= 300.0) {
    category = "Very_Unhealthy";
  }
  else if (AQI >= 300.1){
    category = "Hazardous";
  }

  return category;
}
