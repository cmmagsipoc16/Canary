void connect_wifi(){
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager; 
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    //Default IP: 192.168.4.1

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    //wifiManager.autoConnect("AutoConnectAP");
    //or use this to add password, first parameter is name of access point, second is the password
    wifiManager.autoConnect("ESP_AP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected... yeey! :)");
}

void forget_network(){ 
  WiFiManager wifiManager; 
  wifiManager.resetSettings();
  wifiManager.autoConnect("ESP_AP");

}

void disconnect_wifi(){
  WiFi.persistent(false);      
  WiFi.disconnect();  
  WiFi.persistent(true);  
}
