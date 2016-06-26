#include "application.h"
#include "actions.h"


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
}
