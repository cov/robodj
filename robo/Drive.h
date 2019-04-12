/*
 * +----------------------------------------------------------------------------+
 * |                           Left motor truth table                           |
 * |     Here are some handy tables to show the various modes of operation.     |
 * +----------------------------------------------------------------------------+
 * |  ENA |       IN1      |       IN2      |            Description            |
 * +------+----------------+----------------+-----------------------------------+
 * |  LOW | Not Applicable | Not Applicable | Motor is off                      |
 * +------+----------------+----------------+-----------------------------------+
 * | HIGH |       LOW      |       LOW      | Motor is stopped (brakes)         |
 * +------+----------------+----------------+-----------------------------------+
 * | HIGH |      HIGH      |       LOW      | Motor is on and turning forwards  |
 * +------+----------------+----------------+-----------------------------------+
 * | HIGH |       LOW      |      HIGH      | Motor is on and turning backwards |
 * +------+----------------+----------------+-----------------------------------+
 * | HIGH |      HIGH      |      HIGH      | Motor is stopped (brakes)         |
 * +------+----------------+----------------+-----------------------------------+ 
 */

// Motor wheels
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

// Default carSpeed 0-255
#define carSpeed 255

#define carSpeedInCurveMin 30
#define carSpeedInCurveMax 255

void forward(bool debug = false, int leftSpeed = carSpeed, int rightSpeed = carSpeed){
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) {
    char buf[100];
    sprintf(buf, "Go forward! (%s/%s)", leftSpeed, rightSpeed);
    Serial.println(buf);
  }
}

void back(bool debug = false, int leftSpeed = carSpeed, int rightSpeed = carSpeed){ 
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) {
    char buf[100];
    sprintf(buf, "Go back! (%s/%s)", leftSpeed, rightSpeed);
    Serial.println(buf);
  }
}

void left(bool debug = false, int leftSpeed = carSpeed){
  analogWrite(ENA,leftSpeed);
  analogWrite(ENB,leftSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}

void right(bool debug = false, int rightSpeed = carSpeed){
  analogWrite(ENA,rightSpeed);
  analogWrite(ENB,rightSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}

void stop(bool debug = false){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  if(debug) Serial.println("Stop!");
}

void left_forward(bool debug = false){
  forward(debug, carSpeedInCurveMin, carSpeedInCurveMax);
  if(debug) Serial.println("Go left forward!");
}

void right_forward(bool debug = false){
  forward(debug, carSpeedInCurveMax, carSpeedInCurveMin);
  if(debug) Serial.println("Go right forward!");
}

void left_back(bool debug = false){
  back(debug, carSpeedInCurveMin, carSpeedInCurveMax);
  if(debug) Serial.println("Go left back!");
}

void right_back(bool debug = false){
  back(debug, carSpeedInCurveMax, carSpeedInCurveMin);
  if(debug) Serial.println("Go right back!");
}

void setupDrive() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

