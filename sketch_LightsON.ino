const int buttonPin = 2;
const int porchLightPin = 3;     
const int hallwayLightPin = 4;   

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(porchLightPin, OUTPUT);
  pinMode(hallwayLightPin, OUTPUT);
}

void loop() {

  
  if (digitalRead(buttonPin) == LOW) 
  {

    digitalWrite(porchLightPin, HIGH);
    delay(30000);
    digitalWrite(porchLightPin, LOW);

    digitalWrite(hallwayLightPin, HIGH);
    delay(60000);
    digitalWrite(hallwayLightPin, LOW);
  }
}