#ifndef config_h
#define config_h

#include "application.h"

//time related
int delayed_response_timems = 3*1000;

//State/Flags
enum TYPE { PRESET = 0, CUSTOM, INVALID };
#define MODULES  5

//pin mapping
#define pin_switch D7
#define pin_servoLid D0
#define pin_servoArm D1
#define pin_lidLight D2
#define pin_redLight D3
#define pin_goosesound A0

//Speeds for opening lid
#define fast 10
#define slow 1
#define normal 2

#endif
