#ifndef Constants_h
#define Constants_h

enum FUNCTIONMODE{
  IDLE,
  Bluetooth,
  IRremote,
  ScanForEnemy
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

#endif

