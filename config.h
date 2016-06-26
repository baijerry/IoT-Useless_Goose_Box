#ifndef config_h
#define config_h

#include "application.h"

//State/Flags
enum TYPE { PRESET = 0, CUSTOM, INVALID };
#define MODULES  4

//pin mapping
#define pin_switch 7
#define pin_servoLid1 9
#define pin_servoArm 10
#define pin_lidLight 2
#define pin_redLight 4

#endif
