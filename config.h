#ifndef config_h
#define config_h

#include "application.h"

//time related
#define delayed_response_timems 3*1000;

//State/Flags
enum TYPE { PRESET = 0, CUSTOM, INVALID };
#define MODULES  4


//pin mapping
#define pin_switch 7
#define pin_servoLid 9
#define pin_servoArm 10
#define pin_lidLight 2
#define pin_redLight 4
//Speeds for opening lid
#define fast 10
#define slow 1
#define normal 2
//Max angle for servo motors
#define lidMax 100
#define armMax 119

#endif
