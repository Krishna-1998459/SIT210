#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>   // Library for BH1750 light sensor

// Create BH1750 object
BH1750 lightMeter;

// WiFi credentials
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

// WiFi client for IFTTT
WiFiClient client;

// Threshold in lux (BH1750 gives values in lux, not analog 0–1023)
float threshold = 100.0;   // Adjust based on testing

// Sunlight state tracking
bool sunlight = false;

void setup()
{
  Serial.begin(9600);

  // Start I2C communication
  Wire.begin();

  // Initialize BH1750 sensor
  lightMeter.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
    Serial.println("Still connecting...");
  }

  Serial.println("WiFi Connected");
}

// Function to trigger IFTTT when sunlight starts
void sendSunlightStart()
{
  if (client.connect("maker.ifttt.com", 80))
  {
    client.println("GET /trigger/sunlight_startto/with/key/beKl6rAUXbYMsso2p20o7G HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  }
}

// Function to trigger IFTTT when sunlight stops
void sendSunlightStop()
{
  if (client.connect("maker.ifttt.com", 80))
  {
    client.println("GET /trigger/sunlight_stop/with/key/beKl6rAUXbYMsso2p20o7G HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop()
{
  // Read light intensity in lux
  float lightValue = lightMeter.readLightLevel();

  Serial.print("Light (lux): ");
  Serial.println(lightValue);

  // Sunlight detected
  if (lightValue >= threshold && sunlight == false)
  {
    Serial.println("Sunlight detected");

    digitalWrite(LED_BUILTIN, LOW);

    sendSunlightStart();

    sunlight = true;
  }

  // Sunlight stopped
  if (lightValue < threshold && sunlight == true)
  {
    Serial.println("Sunlight stopped");

    digitalWrite(LED_BUILTIN, HIGH);

    sendSunlightStop();

    sunlight = false;
  }

  delay(2000);
}
