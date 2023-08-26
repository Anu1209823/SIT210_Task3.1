#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>  

#define DHTPIN 2       
#define DHTTYPE DHT11  

char ssid[] = "Anushika's Iphone";     
char pass[] = "anu123456789"; 

unsigned long channelID = 2252138;         
const char *apiKey = "JCBD4UUH23BS18CE";  

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);  // Wait for a few seconds between readings

  float temperature = dht.readTemperature();     
  float humidity = dht.readHumidity();           

  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Failed to read from DHT sensor");
  }
  
  delay(30000); // Wait for 30 seconds
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Initiating WiFi connection...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Successfully established a connection to the WiFi network.");
}

void sendDataToThingSpeak(float temp, float humidity) {
  ThingSpeak.setField(1, temp);       
  ThingSpeak.setField(2, humidity);   
  
  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data has been successfully transmitted to ThingSpeak.");
  } else {
    Serial.println("An issue was encountered during the process of transmitting data to ThingSpeak.");
  }
}