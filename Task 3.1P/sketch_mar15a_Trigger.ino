#include <WiFiNINA.h>

// The light sensor is connected to analog pin A0
#define LIGHT_PIN A0

// WiFi credentials for connecting the Arduino Nano 33 IoT to the network
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

// WiFi client object used to communicate with the IFTTT server
WiFiClient client;

// Threshold value used to decide whether sunlight is present or not
int threshold = 450;

// This variable keeps track of the current sunlight state
// false = no sunlight, true = sunlight detected
bool sunlight = false;

void setup()
{
  // Start serial communication so we can see messages in the Serial Monitor
  Serial.begin(9600);

  // Built-in LED is used as an indicator for sunlight status
  pinMode(LED_BUILTIN, OUTPUT);

  // Inform the user that the board is trying to connect to WiFi
  Serial.println("Connecting to WiFi...");

  // Attempt to connect to the WiFi network
  WiFi.begin(ssid, pass);

  // Keep checking until the connection is successful
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
  }

  // Once connected, print confirmation message
  Serial.println("WiFi Connected");
}

// Function that sends a notification when sunlight is detected
void sendSunlightStart()
{
  // Connect to the IFTTT webhook server
  if (client.connect("maker.ifttt.com", 80))
  {
    // Send HTTP request to trigger the "sunlight_startto" event
    client.println("GET /trigger/sunlight_startto/with/key/beKl6rAUXbYMsso2p20o7G HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  }
}

// Function that sends a notification when sunlight stops
void sendSunlightStop()
{
  // Connect to the IFTTT webhook server
  if (client.connect("maker.ifttt.com", 80))
  {
    // Send HTTP request to trigger the "sunlight_stop" event
    client.println("GET /trigger/sunlight_stop/with/key/beKl6rAUXbYMsso2p20o7G HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop()
{
  // Read the light level from the sensor
  int lightValue = analogRead(LIGHT_PIN);

  // Display the current light value in the Serial Monitor
  Serial.print("Light Value: ");
  Serial.println(lightValue);

  // If the light value is above the threshold and sunlight was previously not detected
  if (lightValue >= threshold && sunlight == false)
  {
    Serial.println("Sunlight detected");

    // Turn OFF the built-in LED
    digitalWrite(LED_BUILTIN, LOW);

    // Send notification to IFTTT
    sendSunlightStart();

    // Update the sunlight state
    sunlight = true;
  }

  // If the light value falls below the threshold and sunlight was previously detected
  if (lightValue < threshold && sunlight == true)
  {
    Serial.println("Sunlight stopped");

    // Turn ON the built-in LED to indicate darkness
    digitalWrite(LED_BUILTIN, HIGH);

    // Send the notification to IFTTT
    sendSunlightStop();

    //It Update the sunlight state
    sunlight = false;
  }

  // Small delay to make readings easier to observe
  delay(2000);
}