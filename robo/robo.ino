/*
   ROBO Downey Junior - Elegoo Smart Robot Car
   Based on Arduino UNO / Elegoo Uno R3
   Version 0.1 April, 2019

   +------------------------------------------------------------------+
   |                          DIY - Control                           |
   +------------------------------------------------------------------+
   | Left Forward | Forward | Right Forward |  |  |    Mind the Gap   |
   |    BT:"a"    |  BT:"f" |     BT:"c"    |  |  |                   |
   |    IR:KEY1   |         |    IR:KEY3    |  |  |                   |
   +--------------+---------+---------------+--+--+-------------------+
   |     Left     |   STOP  |     Right     |  |  |    Enemy Attack   |
   |    BT:"l"    |  BT:"s" |     BT:"r"    |  |  |                   |
   +--------------+---------+---------------+--+--+-------------------+
   |   Left Back  |   Back  |   Right Back  |  |  | Presentation Mode |
   |    BT:"d"    |  BT:"b" |     BT:"e"    |  |  |      (Dance)      |
   |    IR:KEY4   |         |    IR:KEY6    |  |  |                   |
   +--------------+---------+---------------+--+--+-------------------+
*/
// Uncomment following line to disable remote control
#define USE_REMOTE_CONTROL

#ifdef USE_REMOTE_CONTROL
#define USE_HARDWARE_IR
#define USE_HARDWARE_BLUETOOTH
#endif

#include "Globals.h"
#include "Ultrasonic.h"
#include "LineTeacking.h"
#include "Controller.h"
#include "Drive.h"

// #define LED_Pin 13

void delays(unsigned long t) {
  for (unsigned long i = 0; i < t; i++) {
#ifdef USE_HARDWARE_BLUETOOTH
    getBTData();
#endif
#ifdef USE_HARDWARE_IR
    getIRData();
#endif
    delay(1);
  }
}

#ifdef USE_HARDWARE_BLUETOOTH
void bluetooth_mode() {
  if (func_mode == Bluetooth) {
    switch (mov_mode) {
      case FORWARD:       forward();       break;
      case BACK:          back();          break;
      case LEFT:          left();          break;
      case RIGHT:         right();         break;
      case STOP:          stop();          break;
      case LEFT_FORWARD:  left_forward();  break;
      case RIGHT_FORWARD: right_forward(); break;
      case LEFT_BACK:     left_back();     break;
      case RIGHT_BACK:    right_back();    break;
      default: break;
    }
  }
}
#endif

#ifdef USE_HARDWARE_IR
void irremote_mode() {
  if (func_mode == IRremote) {
    switch (mov_mode) {
      case FORWARD:       forward();       break;
      case BACK:          back();          break;
      case LEFT:          left();          break;
      case RIGHT:         right();         break;
      case STOP:          stop();          break;
      case LEFT_FORWARD:  left_forward();  break;
      case RIGHT_FORWARD: right_forward(); break;
      case LEFT_BACK:     left_back();     break;
      case RIGHT_BACK:    right_back();    break;
      default: break;
    }
    if (millis() - IR_PreMillis > 500) {
      mov_mode = STOP;
      IR_PreMillis = millis();
    }
  }
}
#endif

void scan_for_enemy() {
  servo.write(currentServoPos);

  entfernung = getDistance();

  if (entfernung >= 500 || entfernung <= 0) {
    Serial.println("Kein Messwert");
    catchEnemy = false;
  } else {
    if (entfernung < 50) {
      catchEnemy = true;
    } else {
      catchEnemy = false;
    }
    Serial.print(entfernung);
    Serial.print(" cm - ");
  }

  if (!catchEnemy) {
    int newServoPos = 0;
    if (scanClockwise) {
      newServoPos = currentServoPos - GRADE;
    }

    if (!scanClockwise) {
      newServoPos = currentServoPos + GRADE;
    }

    if (newServoPos <= 0 || newServoPos >= 180) {
      if (newServoPos <= 0) {
        newServoPos = 0;
      } else {
        newServoPos = 180;
      }
      scanClockwise = !scanClockwise;
    }
    currentServoPos = newServoPos;
  }

  Serial.print(currentServoPos);
  Serial.println(" Grad");
  delay(100);

}

void attack_enemy() {

  if (currentServoPos > 100 && catchEnemy) {
    left();
    delay(100);
    stop();
    forward();
    delay(50);
  }

  if (currentServoPos < 80 && catchEnemy) {
    right();
    delay(100);
    stop();
    forward();
    delay(500);
  }

}

void setup() {
  setupController();
  setupUltrasonic();
  setupDrive();
  setupLineTeacking();
}

void loop() {
#ifdef USE_HARDWARE_BLUETOOTH
  getBTData();
  bluetooth_mode();
#endif
#ifdef USE_HARDWARE_IR
  getIRData();
  irremote_mode();
#endif
#ifndef USE_REMOTE_CONTROL
  scan_for_enemy();
  attack_enemy();
#endif

}
