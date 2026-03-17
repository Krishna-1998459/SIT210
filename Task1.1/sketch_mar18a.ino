const int buttonPin = 2;
const int led1 = 3;            // Porch (external LED)
const int led2 = LED_BUILTIN; // Hallway (built-in LED)

unsigned long startTime = 0;
bool isRunning = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  checkButton();
  controlLights();
}

void checkButton() {
  if (digitalRead(buttonPin) == HIGH && !isRunning) {
    turnOnLights();
  }
}

//  Turn ON both LEDs at same time
void turnOnLights() {
  digitalWrite(led1, HIGH); // D3 ON
  digitalWrite(led2, HIGH); // Built-in LED ON

  startTime = millis();
  isRunning = true;
}


void controlLights() {
  if (isRunning) {
    unsigned long currentTime = millis();

    // D3 OFF after 30 sec
    if (currentTime - startTime >= 30000) {
      digitalWrite(led1, LOW);
    }

    // Built-in LED OFF after 60 sec
    if (currentTime - startTime >= 60000) {
      digitalWrite(led2, LOW);
      isRunning = false;
    }
  }
}