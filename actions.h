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
    Servo servoLid, servoArm;
    int lid_pos;
    int arm_pos;

  public:
    Action(); //constructor

  ///HELPER FUNCTIONS
    //Move: servo moves from start angle to end angle
    void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed);

    //Shake: Change angle direction very quickly for lid open
    void shakeServo(Servo thisServo);

    //close box lid from any position
    void closeLid();

    //turns off lights, makes sure arm is closed, closes lid
    void reset();

  //5 MAIN ACTION FUNCTIONS
    void actuateLid(char letter);
    void actuateLidLED(char letter);
    void actuateArm(char letter);
    void actuateRedLED(char letter);
    void actuateGooseSound (char letter);

  //PRESET FUNCTIONS
    void preset0();
    void preset1();
    void preset2();
};

#endif
