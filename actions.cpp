#include "application.h"
#include "actions.h"

//Move: servo moves from start angle to end angle
void Action::moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed) {
  bool angleIncr; // boolean to determine if the angle needs to increase or decrease to get to endAngle
  int pos;
  if(startAngle < endAngle) angleIncr = true; //if start angle is smaller than end angle then it must increase

  //Increasing servo angle
  if(angleIncr){
    for (pos = startAngle; pos <= endAngle; pos += angSpeed) { // goes from start angle to end angle in steps of angular speed variable
      thisServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                      // waits 15ms for the servo to reach the position
    }
  }
  //Decreasing servo Angle
  else{
    for(pos = endAngle; pos >= startAngle; pos -= angSpeed) { // goes from end angle to start angle in steps of angular speed variable
      thisServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  delay(15); //delay before next action
}

//Shake: Change angle direction very quickly for lid open
void Action::shakeServo (Servo thisServo) {
  for (int i = 0; i < 4; i++) {//oscillate upen and down for 4 cycles
    moveServo(thisServo, 0, 10, 10); //open quickly (to 10 degrees)
    moveServo(thisServo, 10, 0, 10); //close quickly (to 10 degrees)
  }
}

void Action::actuateLid (char letter) {

}

void Action::actuateLidLED (char letter) {

}

void Action::actuateArm (char letter) {

}

void Action::actuateRedLED (char letter) {

}
