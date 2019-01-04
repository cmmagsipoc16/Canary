int lcd_i; 
int lcd_j; 


void LCD_Param1(){ //Temperature, Humidity, TVOC and CO2
//LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(t); lcd.print("C"); lcd.print(" |"); lcd.print(h); lcd.print("%RH");
  lcd.setCursor(0,1);    
  lcd.print(VOC); lcd.print("ppb"); lcd.print(" | "); lcd.print(CO2); lcd.print("ppm");
  
  lcd_i++;
  if (lcd_i == 10) {
    lcd_i = 0;
    
  //LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PM 2.5 | AQI");
    lcd.setCursor(0,1);    
    lcd.print("Color  |Category");
    delay(2000);    
  
    lcd_k = false;
  }
}

  
void LCD_Param2(){  //PM 2.5, AQI, AQI Category, AQI Color
//LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(PM2_5); lcd.print("ug/L"); lcd.print(" |"); lcd.print("AQI: "); lcd.print(aqi);
  lcd.setCursor(0,1);    
  lcd.print(aqiColor); lcd.print(" | "); lcd.print(aqiCategory);
  
  lcd_j++;
  if (lcd_j == 10) {
    lcd_j = 0;
    
//LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp | Humidity");
    lcd.setCursor(0,1);    
    lcd.print("TVOC | eCO2");
    delay(2000);

    lcd_k = true;
  }  
}
