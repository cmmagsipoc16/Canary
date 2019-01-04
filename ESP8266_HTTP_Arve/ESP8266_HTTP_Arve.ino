//*****Libraries*****//
//For connecting to wifi and http webserver
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
//For software I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/ v1.3.5
#include <SoftwareSerial.h>
#include <ArduinoJson.h>          //version5!

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial pmsSerial(14, 13);

//*****Declarations*****//
#define SDA_PIN  5
#define SCL_PIN  4

int forget;               //To forget previous network
int CO2 = 0;              //Equivalent Carbon Dioxide reading
int VOC = 0;              //Volatile Organic Compound reading
int PM2_5 = 0;            //Fine Particulate Matter reading 2.5 microns in size
float t = 0;              //Temperature reading
float h = 0;              //Humidity reading
String mac;               //MAC address
int aqi = 0;              //Air Quality Index
String aqiColor = "";
String aqiCategory = "";
String data_json; 
String url_device;
bool lcd_k = false;   //For LCD display

//#define SHT31_ADR  0x44;

////*****Setup*****//
void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600);

////Connect to wifi
  connect_wifi();
////To disconnect and forget previous network
  pinMode(13, INPUT);
  
while (WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.println("Waiting for connection");
}
  
////Software I2C  
  Wire.begin(SDA_PIN, SCL_PIN); //(SDA,SCL)
  
////LCD
  lcd.begin(16,2); // LCD is 16x2
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PM 2.5 | AQI");
  lcd.setCursor(0,1);    
  lcd.print("Color  |Category");
  delay(2000);    

////Initialize Sensors
  //For PM2.5  
  Serial.print("Initializing PMS5003...");
  if (initPM25()){
    Serial.println("done.");
  }
  else {
    Serial.println("failed.");
  }
  
  //For SHT31
  Serial.print("Initializing SHT31...");
  if (initSHT31()){
    Serial.println("done.");
  }
  else {
    Serial.println("failed.");
  }

  //For SGP30
  Serial.print("Initializing SGP30...");
   if (initSGP30()){ 
    Serial.println("done.");
    }
  else {
    Serial.println("failed.");

  delay(1000); // Time for opening Serial monitor :)
  }
}


void loop() {

  mac = WiFi.macAddress();
  PM2_5 = readPM25();
  t = ceilf(readTempSHT31() *100) / 100;      //rounded up to 2 decimal places
  h = ceilf(readHumiditySHT31()*100) / 100;   //rounded up to 2 decimal places
  readSGP30_Baseline ();                      //for accurate CO2 and VOC reading
  CO2 = readCO2SGP30();
  VOC = readTVOCSGP30();
  aqi = computeAQI(PM2_5);
  aqiColor = setAQIColor(aqi);
  aqiCategory = setAQICategory (aqi);

//Switching Parameters on LCD Display  
  if(lcd_k){
    LCD_Param1();
  }
  else{
    LCD_Param2();
  }

//Push button to forget previous network
  forget = digitalRead(13);   //reads GPIO 13
  if(forget == LOW){
    forget_network();
    Serial.println("wifi disconnected!");
    ESP.reset();
    delay(2000);
  }

//Display on Serial for debugging
  Serialize(); //see JSON


//HTTP Request
  if(WiFi.status() == WL_CONNECTED){    //Check WiFi connection status
    HTTPClient arve;      //Declare object of class HTTPClient

    arve.begin(http_destination());                                //Specify request destination (for real Arve API)
//    arve.begin("http://jsonplaceholder.typicode.com/posts/1");   //Specify request destination
    
//    arve.addHeader("Content-Type", "text/plain");               //Specify content-type header
    arve.addHeader("Content-Type", "application/json");           //Specify content-type header

    //int postRequest = arve.POST("Hello from Arve");             //Send a POST request
    
    //int patchRequest = arve.PATCH(data_json);                   //Send a PATCH request to dummy API
    int patchRequest = arve.PATCH(data_json);                     //Send a PATCH request to real Arve API 
    //String patchRequest = "{device:" + data_json + "}";         //For Troubleshooting
    String payload = arve.getString();                            //Get the response payload    

    //Serial.println(postRequest); 
    Serial.println(patchRequest);
    Serial.println(payload);
   
    arve.end();   //Close connection
  }
  else{
    Serial.println("Error in WiFi connection");
  }

  delay(10000); //Send data only for every 10 seconds
}
