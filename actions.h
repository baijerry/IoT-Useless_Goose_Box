#ifndef actions_h
#define actions_h

#include "application.h"
#include "config.h"

/* Action Class
 Holds all the possible actions that can be performed by the box

actuateLidLED
 A= on
 B= delayed on
 C= off
 D= flicker

actuateRedLED
 A= on
 B= delayed on
 C= off
 D= flicker

actuateLid
 A= normal
 B= fast
 C= slow
 D= shake

actuateArm
 A= normal
 B= fast
 C= slow
 D= shake

actuateGooseSound
 A= On
 B= Off

 */

class Action {
  private:
    //no  functions, I need all of it for preset stuff
  public:
  //Define static action functions here

  ///HELPER FUNCTIONS
    //Move: servo moves from start angle to end angle
    static void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed);
    //Shake: Change angle direction very quickly for lid open
    static void shakeServo(Servo thisServo);
    //Lid Movement functions (normal, fast, slow, shake)
    static void reset();
    //turns off lights, makes sure arm is closed, closes lid

  //5 MAIN ACTION FUNCTIONS
    static void actuateLid(char letter);
    static void actuateLidLED(char letter);
    static void actuateArm(char letter);
    static void actuateRedLED(char letter);
    static void actuateGooseSound (char letter);

  //PRESET FUNCTIONS
};

#endif
