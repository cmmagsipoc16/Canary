//*****Libraries*****//
#include <FS.h>                    //this needs to be first, or it all crashes and burns...
//For connecting to wifi and mqtt server
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
//For software I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/ v1.3.5
#include <SoftwareSerial.h>
#include <ArduinoJson.h>          //version5!
//For MQTT publish/subscribe
#include <PubSubClient.h>         //https://github.com/knolleary/pubsubclient

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial pmsSerial(14, 13);

WiFiClient espClient;
PubSubClient client(espClient);

//*****Declarations*****//
#define SDA_PIN  5
#define SCL_PIN  4

////For MQTT
//flag for saving data
bool shouldSaveConfig = false;

//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40] = "ip add of hosting Rpi";
char mqtt_port[6] = "1883";
char mqtt_user[20] = "mqtt_user";
char mqtt_pass[20] = "mqtt_pass";

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
bool lcd_k = false;       //For LCD display


////*****Setup*****//
void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600);

  readConfig_Mqtt();      ////Configure MQTT FS before connection
  connect_wifi();         ////Connect to Wifi and MQTT server
  connect_mqttBroker();   ////Connect to mqtt broker

  pinMode(13, INPUT);     ////To disconnect and forget previous network
  
  while (WiFi.status() != WL_CONNECTED){    
    delay(500);
    Serial.println("Waiting for connection");
  }
  
////Software I2C  
  Wire.begin(SDA_PIN, SCL_PIN); //(SDA,SCL)
  
////Initialize LCD
  lcd.begin(16,2);        // LCD is 16x2
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PM 2.5 | AQI");
  lcd.setCursor(0,1);    
  lcd.print("Color  |Category");
  delay(2000);    


////Initialize Sensors
//Initialize PMS5003 (PM2.5)  
  Serial.print("Initializing PMS5003...");
  if (initPM25()){
    Serial.println("done.");
  }
  else {
    Serial.println("failed.");
  }
  
//Initialize SHT31 (Temperature and Humidity Sensor)
  Serial.print("Initializing SHT31...");
  if (initSHT31()){
    Serial.println("done.");
  }
  else {
    Serial.println("failed.");
  }

//Initialize SGP30 (Gas Sensor)
  Serial.print("Initializing SGP30...");
   if (initSGP30()){ 
    Serial.println("done.");
    }
  else {
    Serial.println("failed.");

  delay(1000); // //This is only here to make it easier to catch the startup messages.  It isn't required :)
  }
}


void loop() {
  client.loop();

  mac = WiFi.macAddress();    //arve endpoint needed this before
  
  // Sensor reading
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
  forget = digitalRead(13);
  if(forget == LOW){
    forget_network();
    Serial.println("wifi and mqtt disconnected and forgotten!");
    ESP.reset();
    delay(2000);
  }

  //Display sensor data on Serial for debugging
  Serialize(); //see JSON

  //MQTT Test
  //client.publish("dev/test", "Hello from ESP8266");
  
  //Publish sensor data over MQTT 
  String sensor_data = "{device:" + data_json + "}";
  char sensor_data_buffer[256];
  sensor_data.toCharArray(sensor_data_buffer,256);
  
  client.subscribe("dev/test");                   //Subscribe to topic "dev/test"
  client.publish("dev/test",sensor_data_buffer);  //Publish sensor data to topic "dev/test"

  delay(3000);                     //Publish data every 3 second
}
