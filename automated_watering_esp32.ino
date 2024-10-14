#include <DHTesp.h>
#include <Arduino.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "wifi-name-niyo"
#define WIFI_PASSWORD "password-of-wifi"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDCYsvfU8iCT5iy4-yCxjpRT87yx0VK49E"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://automated-watering-syste-40cb1-default-rtdb.firebaseio.com/" 


FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

String  wifi_local_ip = "";

DHTesp dht;

const int DHTPIN = 2;  // DHT11 data pin connected to GPIO2 on ESP8266

const int soilMoisturePin = A0; // Soil moisture sensor analog pin

void setup() {
 
  
  
  Serial.begin(115200);
  Serial.println("Initializing...");

    WiFi.begin(WIFI_SSID ,WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    wifi_local_ip = WiFi.localIP().toString();

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Sign up */
    if (Firebase.signUp(&config, &auth, "", "")){
      Serial.println("Firebase Signup Authenticated");
      signupOK = true;


    }
    else{
      Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);


   Serial.begin(9600);
   dht.setup(DHTPIN, DHTesp::DHT11); 
   delay(2000); 
}

void loop() {



  // float temperature = dht.getTemperature();
  // float humidity = dht.getHumidity();
  
  // // Check if readings are valid
  // if (!isnan(temperature) && !isnan(humidity)) {
  //   int soilMoisture = analogRead(soilMoisturePin); // Read soil moisture value
    
  //   Serial.print("Temperature: ");
  //   Serial.print(temperature);

  //   Serial.print(" °C, Humidity: ");
  //   Serial.print(humidity);

  //   Serial.print(" %, Soil Moisture: ");
  //   Serial.println(soilMoisture);
  //   Serial.println(" %");

  //   WriteToFirebaseDBString("current_reading", "current_reading_1", "temperature", String(temperature));
  //   WriteToFirebaseDBString("current_reading", "current_reading_1", "humidity", String(humidity));
  //   WriteToFirebaseDBString("current_reading", "current_reading_1", "soil_moisture", String(soilMoisture));

  // } else {
  //   Serial.println("Failed to read from DHT sensor!");
  // }
  
  // delay(5000); 


} // void loop()



void WriteToFirebaseDBString(String table, String id, String field, String field_value){

    // Write an Float number on the database path test/float

    float temp = 0;

    if (Firebase.RTDB.setString(&fbdo, ""+table+"/"+id+"/"+field+"",field_value)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // pag float, setFloat
    // pag int, setInt
    // you can try another data types baka pwede din

} // writeToFirebaseDB
