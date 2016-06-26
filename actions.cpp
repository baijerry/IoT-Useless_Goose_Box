#include "actions.h"

Action::Action(){
  servoLid.attach(pin_servoLid);
  servoArm.attach(pin_servoArm);

  servoArm.write(119);
  delay(500);
  servoLid.write(0);
  delay(500);
}

//-------SERVO MOTOR MOTIONS
//Move: servo moves from start angle to end angle
void Action::moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed) {
  bool angleIncr = false; // boolean to determine if the angle needs to increase or decrease to get to endAngle
  int pos = 0;
  if(startAngle < endAngle) angleIncr = true; //if start angle is smaller than end angle then it must increase

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
void Action::shakeServo (Servo thisServo) {
  closeLid();//close lid before shaking
  for (int i = 0; i < 10; i++) {//oscillate up and down for 10 cycles
    moveServo(thisServo, 0, 15, 15); //open quickly (to 10 degrees)
    moveServo(thisServo, 15, 0, 15); //close quickly (to 10 degrees)
  }
}

//-------5 MAIN FUNCTIONS
//A= on B= delayed on C= off D= flicker
void Action::actuateLidLED (char letter) {
  switch (letter)
  {
    //OFF
    case 'A':
    case 'a':
      digitalWrite(pin_lidLight, LOW);
      break;
    //DELAYED ON
    case 'B':
    case 'b':
      delay(delayed_response_timems);
      digitalWrite(pin_lidLight, LOW);
      break;
    //ON
    case 'C':
    case 'c':
      digitalWrite(pin_lidLight, HIGH);
      break;
    //FLICKER
    case 'D':
    case 'd':
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(pin_lidLight, LOW);
        delay(500);
        digitalWrite(pin_lidLight, HIGH);
        delay(300);
        digitalWrite(pin_lidLight, LOW);
        delay(100);
        digitalWrite(pin_lidLight, HIGH);
        delay(500);
      }
      break;

    default:
      //do nothing
      break;
  }
}

//A= on B= delayed on C= off D= flicker
void Action::actuateRedLED (char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':
      //on
      digitalWrite(pin_redLight, LOW);
      break;

    case 'B':
    case 'b':
      //delayed on
      delay(delayed_response_timems);
      digitalWrite(pin_redLight, LOW);
      break;

    case 'C':
    case 'c':
      digitalWrite(pin_redLight, LOW);
      break;

    case 'D':
    case 'd':
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(pin_redLight, LOW);
        delay(500);
        digitalWrite(pin_redLight, HIGH);
        delay(300);
        digitalWrite(pin_redLight, LOW);
        delay(100);
        digitalWrite(pin_redLight, HIGH);
        delay(500);
      }
      break;

    default:
      //do nothing
      break;
  }

}

//actuateLid:  A= normal B= fast C= slow D= shake
void Action::actuateLid (char letter) {
  switch (letter)
  {
    //NORMAL
    case 'A':
    case 'a':
      moveServo(servoLid, 0, 100, normal);
      break;
    //FAST
    case 'B':
    case 'b':
      moveServo(servoLid, 0, 100, fast);
      break;
    //SLOW
    case 'C':
    case 'c':
      moveServo(servoLid, 0, 100, slow);
      break;
    //SHAKE
    case 'D':
    case 'd':
      shakeServo(servoLid);
      break;

    default:
      //do nothing
      break;
  }

}
//actuateLid:  A= normal B= fast C= slow D= shake
void Action::actuateArm (char letter) {
  switch (letter)
  {
    //NORMAL
    case 'A':
    case 'a':
      moveServo(servoArm, 119, 0, normal);
      delay(750);
      moveServo(servoArm, 0, 119, normal);
      break;
    //FAST
    case 'B':
    case 'b':
      moveServo(servoArm, 119, 0, fast);
      delay(750);
      moveServo(servoArm, 0, 119, fast);
      break;
      //SLOW
    case 'C':
    case 'c':
      moveServo(servoArm, 119, 0, slow);
      delay(750);
      moveServo(servoArm, 0, 119, slow);
      break;
    //SHAKE
    case 'D':
    case 'd':
      shakeServo(servoArm);
      break;

    default:
      //do nothing
      break;
  }

}
//actuateGooseSound: A: sound B: NO sound
void Action::actuateGooseSound(char letter) {
  switch (letter)
  {
    case 'A':
    case 'a':
      //on
      digitalWrite(pin_goosesound, LOW);
      delay(100);
      digitalWrite(pin_goosesound, HIGH);
      break;

    case 'B':
    case 'b':
      //off
      //do nothing
      break;

    default:
      //do nothing
      break;
  }

}

//------HELPER FUNCTIONS
//Close box lid
void Action::closeLid(){
  servoLid.write(0);  //set servo angle to 0/close lid
  delay(500);
}

//reset lights arm and lid
void Action::reset(){
  digitalWrite(pin_lidLight, HIGH);
  digitalWrite(pin_redLight, HIGH);
  digitalWrite(pin_goosesound, HIGH);

  servoArm.write(119);
  delay(500);

  closeLid();
}
