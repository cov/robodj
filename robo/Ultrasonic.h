#include <Servo.h>

#define ECHO_PIN  A4  // Ultrasonic Sensor In (analog)
#define TRIG_PIN  A5  // Ultrasonic Sensor Out (analog)

#define GRADE 20 // Winkel in Grad per Tick

Servo servo;

// Use Ultrasonic Sesor to get distance

int currentServoPos = 90, entfernung = 0, timeRange = 0;
bool scanClockwise = true;
bool catchEnemy = false;

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delay(5);
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);
  timeRange = pulseIn(ECHO_PIN, HIGH);
  return (timeRange / 2) * 0.03432;
}

void setupUltrasonic() {
  servo.attach(3,500,2400);// 500: 0 degree  2400: 180 degree
  servo.write(90);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}
