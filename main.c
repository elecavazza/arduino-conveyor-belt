#include <NewPing.h>

#define TRIGGER_PIN  3
#define ECHO_PIN     4
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int switchPin = 2;
const int motorPin = 9;
int switchState = 0;
int detectCount = 0;
bool buttonHeld = false;
bool lastDetection = false;

unsigned long lastDetectionTime = 0;
const unsigned long detectionInterval = 250;

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(motorPin, HIGH);

  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  int distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (switchState == HIGH) {
    buttonHeld = true;
  } else {
    buttonHeld = false;
  }

  unsigned long currentTime = millis();
  if (currentTime - lastDetectionTime >= detectionInterval) {
    lastDetection = false;
  }

  bool currentDetection = distance < 10;
  if (currentDetection && !lastDetection) {
    lastDetectionTime = currentTime;
    lastDetection = true;
    if (buttonHeld) {
      detectCount++;
      Serial.println("Detected! Count: " + String(detectCount));
    } else {
      detectCount++;
      Serial.println("Detected! Count: " + String(detectCount));
    }
  }

  int countLimit = buttonHeld ? 6 : 3;
  if (detectCount >= countLimit) {
    digitalWrite(motorPin, LOW);
    Serial.println("Motor stopped for 5 seconds");
    delay(5000);
    detectCount = 0;
    digitalWrite(motorPin, HIGH);
    Serial.println("Motor restarted");
  }
}
