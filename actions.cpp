#include "actions.h"

//Move: servo moves from start angle to end angle
void Action::moveServo (Servo thisServo, int servopin, int startAngle, int endAngle, int angSpeed) {
  bool angleIncr; // boolean to determine if the angle needs to increase or decrease to get to endAngle
  int pos;
  thisServo.attach(servopin);
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
void Action::shakeServo (Servo thisServo, int servopin) {
  for (int i = 0; i < 4; i++) {//oscillate upen and down for 4 cycles
    moveServo(thisServo, servopin, 0, 10, 10); //open quickly (to 10 degrees)
    moveServo(thisServo, servopin, 10, 0, 10); //close quickly (to 10 degrees)
  }
}

void Action::actuateLidLED (char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':
      //on
      digitalWrite(pin_lidLight, HIGH);
      break;

    case 'B':
    case 'b':
      //delayed on

      break;

    case 'C':
    case 'c':

      break;

    case 'D':
    case 'd':

      break;

    default:
      //do nothing
      break;
  }

  //wrapup

}

void Action::actuateRedLED (char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':

      break;

    case 'B':
    case 'b':

      break;

    case 'C':
    case 'c':

      break;

    case 'D':
    case 'd':

      break;

    default:
      //do nothing
      break;
  }

  //wrapup

}

void Action::actuateLid (char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':

      break;

    case 'B':
    case 'b':

      break;

    case 'C':
    case 'c':

      break;

    case 'D':
    case 'd':

      break;

    default:
      //do nothing
      break;
  }

  //wrapup
}

void Action::actuateArm (char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':

      break;

    case 'B':
    case 'b':

      break;

    case 'C':
    case 'c':

      break;

    case 'D':
    case 'd':

      break;

    default:
      //do nothing
      break;
  }

  //wrapup
}

void Action::actuateGooseSound(char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':

      break;

    case 'B':
    case 'b':

      break;

    default:
      //do nothing
      break;
  }

  //wrapup
}

void Action::reset(){

}
