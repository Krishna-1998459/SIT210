#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

// Create light sensor object
BH1750 lightMeter;

// WiFi details
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

WiFiClient client;

// Simple light threshold
float threshold = 100.0;
float thresholdLow = 90;
// Track current sunlight state
bool sunlight = false;


bool startSent = false;
bool stopSent = false;

void setup()
{
  Serial.begin(9600);

  // Start sensor
  Wire.begin();
  lightMeter.begin();

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Still connecting...");
  }

  Serial.println("WiFi Connected");
}

// Send email when sunlight starts
void sendSunlightStart()
{
  if (client.connect("maker.ifttt.com", 80))
  {
    client.println("GET /trigger/sunlight_start/with/key/beKl6rAUXbYMsso2p20o7G HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  }
}

// Send email when sunlight stops
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
  // Read light value
  float lightValue = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.println(lightValue);

  // If light is high and we haven't sent start email yet
  if (lightValue >= threshold && sunlight == false && startSent == false)
  {
    Serial.println("Sunlight started");

    sendSunlightStart();   // send email once
    sunlight = true;
    startSent = true;
  }

  // If light goes low and we haven't sent stop email yet
  if (lightValue < threshold && sunlight == true && stopSent == false)
  {
    Serial.println("Sunlight stopped");

    sendSunlightStop();   // send email once
    sunlight = false;
    stopSent = true;
  }

  delay(2000); // wait 2 seconds before next reading
}
