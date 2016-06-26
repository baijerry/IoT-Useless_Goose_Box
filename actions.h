#ifndef actions_h
#define actions_h


/* Action Class
 * Holds all the possible actions that can be performed by the box
 */

class Action {
  private:

  public:
    //Define static action functions here

    //Move servo motor from start angle to end angle
    static void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed);
    //Shake: Change angle direction very quickly for lid open
    static void shakeServo();
    //Lid Movement functions (normal, fast, slow, shake)
    static void actuateLid(char letter);
    //ToDo: Lid LED functions (on, delayed on, off, flicker)
    static void actuateLidLED();
    //ToDo: Arm Movement (normal, fast, slow, shake)
    static void actuateArm();
    //ToDo: Goose Red LED functions (on, delayed on, off, flicker)
    static void actuateRedLED();
    //ToDo: Define preset functions

};

#endif
