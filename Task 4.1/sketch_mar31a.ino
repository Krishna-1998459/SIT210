// Pins
#define PIR 2
#define LED1 6
#define LED2 7

bool ledState = false;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(PIR, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  Serial.println("Motion setup");
}

void loop() {
  int motion = digitalRead(PIR);

  if (motion == HIGH) {
    if (!ledState) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      ledState = true;
      Serial.println("Motion detected → Lights ON");
    }
  } 
  else {
    if (ledState) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      ledState = false;
      Serial.println("Motion not detected → Lights OFF");
    }
  }

  delay(1500);  // small delay for stability
}