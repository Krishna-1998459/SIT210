#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

WiFiClient client;

unsigned long channelID = 3288898;
const char * writeAPIKey = "2NM7B5GC1OWGGQK3";

int lightPin = A0;

int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(9600);
  dht.begin();

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);
    delay(5000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi Connected");

  ThingSpeak.begin(client);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int lightValue = analogRead(lightPin);

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Light Level: ");
  Serial.println(lightValue);

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, lightValue);
  ThingSpeak.setField(3, humidity);

  ThingSpeak.writeFields(channelID, writeAPIKey);

  Serial.println("-----------------------");

  delay(2000);
}