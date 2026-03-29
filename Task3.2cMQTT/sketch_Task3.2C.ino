#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi
char ssid[] = "Manoj Krishna iphone";
char pass[] = "manojkrishna";

// MQTT
const char* broker = "broker.emqx.io";
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Pins
#define TRIG_PIN 2
#define ECHO_PIN 3
#define LED1 6
#define LED2 7

#define THRESHOLD 30

String myName = "Manoj";

// To track state
bool lastStateWave = false;

// MQTT receive
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received: ");
  Serial.println(message);

  if (String(topic) == "ES/Wave") {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }

  if (String(topic) == "ES/Pat") {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Connect WiFi (simple way)
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // MQTT setup
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop() {

  // -------- MQTT Connection --------
  if (!client.connected()) {
    Serial.print("Connecting MQTT...");

    if (client.connect("ManojClient")) {
      Serial.println("Connected!");

      client.subscribe("ES/Wave");
      client.subscribe("ES/Pat");

    } else {
      Serial.println("Failed, retrying...");
      delay(2000);
      return;
    }
  }

  client.loop();

  // -------- Ultrasonic Sensor --------
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // -------- Logic --------
  if (distance > THRESHOLD) {

    // WAVE
    if (!lastStateWave) {
      client.publish("ES/Wave", myName.c_str());
      Serial.println("Published: ES/Wave");
      lastStateWave = true;
    }

  } else {

    // PAT
    if (lastStateWave) {
      client.publish("ES/Pat", myName.c_str());
      Serial.println("Published: ES/Pat");
      lastStateWave = false;
    }
  }

  delay(500);
}