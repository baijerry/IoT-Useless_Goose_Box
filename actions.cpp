#include "actions.h"
Servo servoLid, servoArm;

//Move: servo moves from start angle to end angle
void Action::moveServo (Servo thisServo, int servopin, int startAngle, int endAngle, int angSpeed) {
  bool angleIncr = false; // boolean to determine if the angle needs to increase or decrease to get to endAngle
  int pos;
  thisServo.attach(servopin);
  if(startAngle < endAngle) angleIncr = true; //if start angle is smaller than end angle then it must increase

  //set servo to start angle
  thisServo.write(startAngle);

  //Increasing servo angle
  if(angleIncr){
    for (pos = startAngle; pos <= endAngle; pos += angSpeed) { // goes from start angle to end angle in steps of angular speed variable
      thisServo.write(pos);           // tell servo to go to position in variable 'pos'
      delay(15);                      // waits 15ms for the servo to reach the position
    }
  }
  //Decreasing servo Angle
  else{
    for(pos = startAngle; pos >= endAngle; pos -= angSpeed) { // goes from end angle to start angle in steps of angular speed variable
      thisServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
}

//Shake: move to closed position and shake lid
void Action::shakeServo (Servo thisServo, int servopin) {
  closeLid();//close lid before shaking
  for (int i = 0; i < 10; i++) {//oscillate up and down for 10 cycles
    moveServo(thisServo, servopin, 0, 15, 15); //open quickly (to 10 degrees)
    moveServo(thisServo, servopin, 15, 0, 15); //close quickly (to 10 degrees)
  }
}

//Close box lid
void Action::closeLid(){
  servoLid.write(0);  //set servo angle to 0/close lid
}
//actuateLidLED: A= on B= delayed on C= off D= flicker
void Action::actuateLidLED (char letter) {
  switch (letter)
  {
    //ON
    case 'A':
    case 'a':
      digitalWrite(pin_lidLight, HIGH);
      break;
    //DELAYED ON
    case 'B':
    case 'b':
      delay(3000);//delay 3 sec then turn on
      digitalWrite(pin_lidLight, HIGH);
      break;
    //OFF
    case 'C':
    case 'c':
      //do nothing
      break;
    //FLICKER
    case 'D':
    case 'd':
    for (int i = 0; i < 20; i++){
      digitalWrite(pin_lidLight, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);              // wait for a second
      digitalWrite(pin_lidLight, LOW);    // turn the LED off by making the voltage LOW
      delay(100);
    }
      break;

    default:
      //do nothing
      break;
  }

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

//actuateLid:  A= normal B= fast C= slow D= shake
void Action::actuateLid (char letter) {
  switch (letter)
  {
    //NORMAL
    case 'A':
    case 'a':
      moveServo(servoLid, pin_servoLid, 0, lidMax, normal);
      break;
    //FAST
    case 'B':
    case 'b':
      moveServo(servoLid, pin_servoLid, 0, lidMax, fast);
      break;
    //SLOW
    case 'C':
    case 'c':
      moveServo(servoLid, pin_servoLid, 0, lidMax, slow);
      break;
    //SHAKE
    case 'D':
    case 'd':
      shakeServo(servoLid, pin_servoLid);
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
    //NORMAL
    case 'A':
    case 'a':
      moveServo(servoArm, pin_servoArm, 0, armMax, normal);
      break;
    //FAST
    case 'B':
    case 'b':
      moveServo(servoArm, pin_servoArm, 0, armMax, fast);
      break;
      //SLOW
    case 'C':
    case 'c':
      moveServo(servoArm, pin_servoArm, 0, armMax, slow);
      break;
    //SHAKE
    case 'D':
    case 'd':
      shakeServo(servoArm, pin_servoArm);
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
