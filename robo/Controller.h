/*
 * +------------------------------------------------------------------+
 * |                          DIY - Control                           |
 * +------------------------------------------------------------------+
 * | Left Forward | Forward | Right Forward |  |  |    Mind the Gap   |
 * |    BT:"a"    |  BT:"f" |     BT:"c"    |  |  |                   |
 * |    IR:KEY1   |         |    IR:KEY3    |  |  |                   |
 * +--------------+---------+---------------+--+--+-------------------+
 * |     Left     |   STOP  |     Right     |  |  |    Enemy Attack   |
 * |    BT:"l"    |  BT:"s" |     BT:"r"    |  |  |       BT:"g"      |
 * |              |         |               |  |  |      IR:KEY7      |
 * +--------------+---------+---------------+--+--+-------------------+
 * |   Left Back  |   Back  |   Right Back  |  |  | Presentation Mode |
 * |    BT:"d"    |  BT:"b" |     BT:"e"    |  |  |      (Dance)      |
 * |    IR:KEY4   |         |    IR:KEY6    |  |  |                   |
 * +--------------+---------+---------------+--+--+-------------------+
 */
#include <IRremote.h>

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

#include "Globals.h"

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long IR_PreMillis;
unsigned long LT_PreMillis;

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
      case 'g': func_mode = ScanForEnemy;                       break;
      default:  break;
    } 
  }
}

void getIRData() {
  if (irrecv.decode(&results)) {
    IR_PreMillis = millis();
    switch (results.value) {
      case f:    func_mode = IRremote; mov_mode = FORWARD;       break;
      case b:    func_mode = IRremote; mov_mode = BACK;          break;
      case l:    func_mode = IRremote; mov_mode = LEFT;          break;
      case r:    func_mode = IRremote; mov_mode = RIGHT;         break;
      case s:    func_mode = IRremote; mov_mode = STOP;          break;
      case KEY1: func_mode = IRremote; mov_mode = LEFT_FORWARD;  break;
      case KEY3: func_mode = IRremote; mov_mode = RIGHT_FORWARD; break;
      case KEY4: func_mode = IRremote; mov_mode = LEFT_BACK;     break;
      case KEY6: func_mode = IRremote; mov_mode = RIGHT_BACK;    break;
      case KEY7: func_mode = ScanForEnemy;                       break;
      default: break;
    }
    irrecv.resume();
  }
}

void setupController() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}
