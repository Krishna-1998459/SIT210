#include <Wire.h>

#include <BH1750.h>



BH1750 lightMeter;

#include <Wire.h>        // Library for I2C communication
#include <BH1750.h>      // Library for BH1750 light sensor

// Create BH1750 sensor object
BH1750 lightMeter;

// Pin definitions
const int PIR_PIN = 2;   // PIR motion sensor connected to pin 2
const int LED1 = 6;      // First LED connected to pin 6
const int LED2 = 7;      // Second LED connected to pin 7

// Variable used to detect motion from interrupt
volatile bool motionDetected = false;

void setup()
{
  // Start serial communication
  Serial.begin(9600);

  // Small startup delay
  delay(1500);

  // Set PIR sensor as input
  pinMode(PIR_PIN, INPUT);

  // Set LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Start I2C communication
  Wire.begin();

  // Start BH1750 sensor
  lightMeter.begin();

  // Attach interrupt to PIR sensor
  // When motion is detected, motionISR() will run
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);

  // Startup messages
  Serial.println("Smart Light System Started");
  Serial.println("Monitoring motion and brightness...");
}

void loop()
{
  // Read current light level from BH1750 sensor
  float lightValue = lightMeter.readLightLevel();

  // Print lux value to Serial Monitor
  Serial.print("Current Lux Value: ");
  Serial.println(lightValue);

  // Check if motion was detected
  if (motionDetected == true)
  {
    // Reset motion flag
    motionDetected = false;

    Serial.println("Motion has been detected");

    // If environment is dark
    if (lightValue < 50)
    {
      // Turn ON both LEDs
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);

      Serial.println("Dark room detected - LEDs turned ON");

      // Keep LEDs ON for 5 seconds
      delay(5000);

      // Turn OFF LEDs
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);

      Serial.println("LEDs turned OFF");
    }
    else
    {
      // Do not turn on LEDs if room is already bright
      Serial.println("Enough light detected - LEDs stay OFF");
    }
  }

  // Small delay for stable readings
  delay(500);
}

// Interrupt Service Routine (ISR)
// Runs automatically when PIR sensor detects motion
void motionISR()
{
  motionDetected = true;
}
