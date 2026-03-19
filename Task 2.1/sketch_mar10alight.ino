#include <WiFiNINA.h>      // Library for WiFi connection (Arduino Nano 33 IoT)
#include <ThingSpeak.h>    // Library to send data to ThingSpeak cloud
#include <DHT.h>           // Library for DHT temperature & humidity sensor

//  SENSOR SETUP 
// DHT11 is connected to pin 3
#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// LDR (light sensor) connected to analog pin A0
int lightPin = A0;

//  WIFI DETAILS 
// Using my mobile hotspot for internet connection
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

// WiFi status tracker
int status = WL_IDLE_STATUS;
WiFiClient client;

//  THINGSPEAK DETAILS
// Channel and API key to upload sensor data
unsigned long channelID = 3288898;
const char * writeAPIKey = "2NM7B5GC1OWGGQK3";

// FUNCTION DECLARATIONS 
void connectWiFi();
void readSensors(float &temp, float &hum, int &light);
void printData(float temp, float hum, int light);
void sendToThingSpeak(float temp, float hum, int light);

// ------------------- SETUP -------------------
void setup() {
  Serial.begin(9600);   // Start serial monitor for debugging
  dht.begin();          // Start DHT sensor

  connectWiFi();        // Connect to WiFi first
  ThingSpeak.begin(client);  // Start ThingSpeak communication
}

// ------------------- MAIN LOOP -------------------
void loop() {
  float temperature, humidity;
  int lightValue;

  // Read all sensor values together
  readSensors(temperature, humidity, lightValue);

  // Sometimes DHT gives invalid readings, so checking it
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor");
    return;  // Skip this loop if data is invalid
  }

  // Print values to Serial Monitor (for testing/debugging)
  printData(temperature, humidity, lightValue);

  // Send data to ThingSpeak cloud
  sendToThingSpeak(temperature, humidity, lightValue);

  Serial.println("-----------------------");

  // Small delay to avoid spamming ThingSpeak
  delay(2000);
}

// ------------------- FUNCTIONS -------------------

// Handles WiFi connection
void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Keep trying until connected
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);  // wait before retrying
    Serial.println("Still trying to connect...");
  }

  Serial.println("WiFi Connected successfully");
}

// Reads temperature, humidity, and light values
void readSensors(float &temp, float &hum, int &light) {
  temp = dht.readTemperature();   // Get temperature in Celsius
  hum = dht.readHumidity();       // Get humidity percentage
  light = analogRead(lightPin);   // Read light level (0–1023)
}

// Displays sensor values in Serial Monitor
void printData(float temp, float hum, int light) {
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Light Level: ");
  Serial.println(light);
}

// Sends sensor data to ThingSpeak
void sendToThingSpeak(float temp, float hum, int light) {
  // Assign values to fields in ThingSpeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, light);
  ThingSpeak.setField(3, hum);

  // Upload data
  ThingSpeak.writeFields(channelID, writeAPIKey);
}