/*
 * ROBO Downey Junior - Elegoo Smart Robot Car
 * Based on Arduino UNO / Elegoo Uno R3
 * Version 0.1 April, 2019
 * 
 * +------------------------------------------------------------------+
 * |                          DIY - Control                           |
 * +------------------------------------------------------------------+
 * | Left Forward | Forward | Right Forward |  |  |    Mind the Gap   |
 * |    BT:"a"    |  BT:"f" |     BT:"c"    |  |  |                   |
 * |    IR:KEY1   |         |    IR:KEY3    |  |  |                   |
 * +--------------+---------+---------------+--+--+-------------------+
 * |     Left     |   STOP  |     Right     |  |  |    Enemy Attack   |
 * |    BT:"l"    |  BT:"s" |     BT:"r"    |  |  |                   |
 * +--------------+---------+---------------+--+--+-------------------+
 * |   Left Back  |   Back  |   Right Back  |  |  | Presentation Mode |
 * |    BT:"d"    |  BT:"b" |     BT:"e"    |  |  |      (Dance)      |
 * |    IR:KEY4   |         |    IR:KEY6    |  |  |                   |
 * +--------------+---------+---------------+--+--+-------------------+
 */

#include <IRremote.h>
#include <Servo.h>  

#define f 16736925  // FORWARD
#define b 16754775  // BACK
#define l 16720605  // LEFT
#define r 16761405  // RIGHT
#define s 16712445  // STOP
#define KEY1 16738455 //Line Teacking mode
#define KEY2 16750695 //Obstacles Avoidance mode
#define KEY3 16756815
#define KEY4 16724175
#define KEY5 16718055
#define KEY6 16743045
#define KEY7 16716015
#define KEY8 16726215
#define KEY9 16734885
#define KEY0 16730805
#define KEY_STAR 16728765
#define KEY_HASH 16732845

#define RECV_PIN  12  // IR Receiver
#define ECHO_PIN  A4  // Ultrasonic Sensor In (analog)
#define TRIG_PIN  A5  // Ultrasonic Sensor Out (analog)

// Motor wheels
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
// +----------------------------------------------------------------------------+
// |                           Left motor truth table                           |
// |     Here are some handy tables to show the various modes of operation.     |
// +----------------------------------------------------------------------------+
// |  ENA |       IN1      |       IN2      |            Description            |
// +------+----------------+----------------+-----------------------------------+
// |  LOW | Not Applicable | Not Applicable | Motor is off                      |
// +------+----------------+----------------+-----------------------------------+
// | HIGH |       LOW      |       LOW      | Motor is stopped (brakes)         |
// +------+----------------+----------------+-----------------------------------+
// | HIGH |      HIGH      |       LOW      | Motor is on and turning forwards  |
// +------+----------------+----------------+-----------------------------------+
// | HIGH |       LOW      |      HIGH      | Motor is on and turning backwards |
// +------+----------------+----------------+-----------------------------------+
// | HIGH |      HIGH      |      HIGH      | Motor is stopped (brakes)         |
// +------+----------------+----------------+-----------------------------------+


// #define LED_Pin 13

#define LineTeacking_Pin_Right  10
#define LineTeacking_Pin_Middle 4
#define LineTeacking_Pin_Left   2
#define LineTeacking_Read_Right   !digitalRead(10)
#define LineTeacking_Read_Middle  !digitalRead(4)
#define LineTeacking_Read_Left    !digitalRead(2)

// Default carSpeed 0-255
#define carSpeed 255

#define carSpeedInCurveMin 30
#define carSpeedInCurveMax 255

Servo servo;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long IR_PreMillis;
unsigned long LT_PreMillis;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

enum FUNCTIONMODE{
  IDLE,
  Bluetooth,
  IRremote
} func_mode = IDLE;

enum MOTIONMODE {
  STOP,
  FORWARD,
  BACK,
  LEFT,
  RIGHT,
  LEFT_FORWARD,
  RIGHT_FORWARD,
  LEFT_BACK,
  RIGHT_BACK
} mov_mode = STOP;

void delays(unsigned long t) {
  for(unsigned long i = 0; i < t; i++) {
    getBTData();
    getIRData();
    delay(1);
  }
}

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

void getBTData() {
  if(Serial.available()) {
    switch(Serial.read()) {
      case 'f': func_mode = Bluetooth; mov_mode = FORWARD;       break;
      case 'b': func_mode = Bluetooth; mov_mode = BACK;          break;
      case 'l': func_mode = Bluetooth; mov_mode = LEFT;          break;
      case 'r': func_mode = Bluetooth; mov_mode = RIGHT;         break;
      case 's': func_mode = Bluetooth; mov_mode = STOP;          break;
      case 'a': func_mode = Bluetooth; mov_mode = LEFT_FORWARD;  break;
      case 'c': func_mode = Bluetooth; mov_mode = RIGHT_FORWARD; break;
      case 'd': func_mode = Bluetooth; mov_mode = LEFT_BACK;     break;
      case 'e': func_mode = Bluetooth; mov_mode = RIGHT_BACK;    break;
      default:  break;
    } 
  }
}

void getIRData() {
  if (irrecv.decode(&results)){ 
    IR_PreMillis = millis();
    switch(results.value){
      case f:    func_mode = IRremote; mov_mode = FORWARD;       break;
      case b:    func_mode = IRremote; mov_mode = BACK;          break;
      case l:    func_mode = IRremote; mov_mode = LEFT;          break;
      case r:    func_mode = IRremote; mov_mode = RIGHT;         break;
      case s:    func_mode = IRremote; mov_mode = STOP;          break;
      case KEY1: func_mode = IRremote; mov_mode = LEFT_FORWARD;  break;
      case KEY3: func_mode = IRremote; mov_mode = RIGHT_FORWARD; break;
      case KEY4: func_mode = IRremote; mov_mode = LEFT_BACK;     break;
      case KEY6: func_mode = IRremote; mov_mode = RIGHT_BACK;    break;
      default: break;
    }
    irrecv.resume();
  }
}

void bluetooth_mode() {
  if(func_mode == Bluetooth){
    switch(mov_mode){
      case FORWARD: forward();  break;
      case BACK:    back();     break;
      case LEFT:    left();     break;
      case RIGHT:   right();    break;
      case STOP:    stop();     break;
      case LEFT_FORWARD:  forward(true, carSpeedInCurveMin, carSpeedInCurveMax); break;
      case RIGHT_FORWARD: forward(true, carSpeedInCurveMax, carSpeedInCurveMin); break;
      case LEFT_BACK:     back(true, carSpeedInCurveMin, carSpeedInCurveMax); break;
      case RIGHT_BACK:    back(true, carSpeedInCurveMax, carSpeedInCurveMin); break;
      default: break;
    }
  }
}

void irremote_mode() {
  if(func_mode == IRremote){
    switch(mov_mode){
      case FORWARD: forward();  break;
      case BACK:    back();     break;
      case LEFT:    left();     break;
      case RIGHT:   right();    break;
      case STOP:    stop();     break;
      case LEFT_FORWARD:  forward(true, carSpeedInCurveMin, carSpeedInCurveMax); break;
      case RIGHT_FORWARD: forward(true, carSpeedInCurveMax, carSpeedInCurveMin); break;
      case LEFT_BACK:     back(true, carSpeedInCurveMin, carSpeedInCurveMax); break;
      case RIGHT_BACK:    back(true, carSpeedInCurveMax, carSpeedInCurveMin); break;
      default: break;
    }
    if(millis() - IR_PreMillis > 500){
      mov_mode = STOP;
      IR_PreMillis = millis();
    }
  }   
}

void setup() {
  Serial.begin(9600);
  servo.attach(3,500,2400);// 500: 0 degree  2400: 180 degree
  servo.write(90);
  irrecv.enableIRIn();
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LineTeacking_Pin_Right, INPUT);
  pinMode(LineTeacking_Pin_Middle, INPUT);
  pinMode(LineTeacking_Pin_Left, INPUT);
}

void loop() {
  getBTData();
  getIRData();
  bluetooth_mode();
  irremote_mode();
}
