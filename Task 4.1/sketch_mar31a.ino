// Pins
#define PIR 2
#define LED1 6
#define LED2 7

volatile bool motionDetected = false;  // updated by interrupt
bool ledState = false;

// runs once
void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(PIR, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // attach interrupt to PIR
  attachInterrupt(digitalPinToInterrupt(PIR), detectMotion, RISING);

  Serial.println("Motion setup");
}

// main loop
void loop() {

  // if motion detected by interrupt
  if (motionDetected) {
    motionDetected = false;

    if (!ledState) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      ledState = true;
      Serial.println("Motion detected → Lights ON");
    }
  } 
  else {
    // no motion → turn off lights
    if (ledState) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      ledState = false;
      Serial.println("Motion not detected → Lights OFF");
    }
  }

  delay(200);  // small delay
}

// interrupt function
void detectMotion() {
  motionDetected = true;  
