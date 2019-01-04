void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i=0; i<length;i++){
    Serial.print((char)payload[i]);    
  }

  Serial.println();
  Serial.println("---------------------");
}

//void cert_key() {
//  if (!SPIFFS.begin()) {
//    Serial.println("Failed to mount file system");
//    return;
//  }
//}

void connect_AWS_MQTT(){
 
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);

  
  while(!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect(aws_thing)){
      Serial.println("Connected to MQTT broker!");
    }
    else{
      Serial.print("Failed with state ");
      Serial.println(client.state());
      // Forget wifi and mqtt credentials and reset ESP
      forget_network();
      Serial.println("wifi and mqtt disconnected and forgotten!");
      ESP.reset();
      delay(2000);
  
    }
  }
}

void readAWS_cert_key(){
    //clean FS for testing 
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

    // Load certificate file
    File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt with your uploaded file name
    if (!cert) {
      Serial.println("Failed to open cert file");
    }
    else
      Serial.println("Success to open cert file"); 
    delay(1000);

    if (espClient.loadCertificate(cert))
      Serial.println("cert loaded");
    else
      Serial.println("cert not loaded");

    // Load private key file
    File private_key = SPIFFS.open("/private.der", "r"); //replace private with your uploaded file name
    if (!private_key) {
     Serial.println("Failed to open private cert file");
    }
    else
      Serial.println("Success to open private cert file");

    delay(1000);

    if (espClient.loadPrivateKey(private_key))
      Serial.println("private key loaded");
    else
      Serial.println("private key not loaded");


    /*
      // Load CA file
      File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
      if (!ca) {
        Serial.println("Failed to open ca ");
      }
      else
      Serial.println("Success to open ca");
      delay(1000);
      if(espClient.loadCACert(ca))
      Serial.println("ca loaded");
      else
      Serial.println("ca failed");
    */
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
  }
}


void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void readConfig_Mqtt(){
    //clean FS for testing 
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}
