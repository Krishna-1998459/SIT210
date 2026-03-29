#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>   // Library for BH1750 light sensor

// -------- BH1750 --------
BH1750 lightMeter;

// -------- WiFi --------
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

WiFiClient client;

// -------- Thresholds (Hysteresis) --------
float thresholdHigh = 110.0; // Sunlight ON
float thresholdLow  = 90.0;  // Sunlight OFF

// -------- State --------
bool sunlight = false;

// -------- Cooldown --------
unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 60000; // 60 seconds

void setup()
{
  Serial.begin(9600);

  Wire.begin();
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

// -------- IFTTT Trigger: Sunlight START --------
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

// -------- IFTTT Trigger: Sunlight STOP --------
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
  float lightValue = lightMeter.readLightLevel();

  Serial.print("Light (lux): ");
  Serial.println(lightValue);

  unsigned long currentTime = millis();

  // -------- Sunlight DETECTED --------
  if (lightValue >= thresholdHigh && sunlight == false && (currentTime - lastTriggerTime > cooldown))
  {
    Serial.println("Sunlight detected");

    digitalWrite(LED_BUILTIN, LOW);

    sendSunlightStart();

    sunlight = true;
    lastTriggerTime = currentTime;
  }

  // -------- Sunlight STOPPED --------
  if (lightValue <= thresholdLow && sunlight == true && (currentTime - lastTriggerTime > cooldown))
  {
    Serial.println("Sunlight stopped");

    digitalWrite(LED_BUILTIN, HIGH);

    sendSunlightStop();

    sunlight = false;
    lastTriggerTime = currentTime;
  }

  delay(2000);
}
