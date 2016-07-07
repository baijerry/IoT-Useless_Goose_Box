#include "application.h"
#include "config.h"

#include "ArduinoJson.h"
#include "SparkIntervalTimer.h"
#include <string.h>

/* ACTIONS

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

//-------------------------------
// VARIABLES
//-------------------------------
TYPE typeFlag;
IntervalTimer threadTimer;
Servo servoLid, servoArm;
int lid_pos;
int arm_pos;

//arrays
String customArray[6];

int customArrayNumItems = 6;
int presetArrayNumItems = 0;

//declare
void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed);
void shakeServo(Servo thisServo);
void closeLid();
void m_reset();
void actuateLid(char letter);
void actuateLidLED(char letter);
void actuateArm(char letter);
void actuateRedLED(char letter);
void actuateGooseSound (char letter);

//-------------------------------
// SETUP
//-------------------------------
void setup() {
  Particle.function("jsonParser", jsonParser);
  Serial.begin(9600);
  Serial.println("Connected to serial monitor.");

  //pin init
  pinMode(pin_switch, INPUT);
  pinMode(pin_lidLight, OUTPUT);
  pinMode(pin_redLight, OUTPUT);
  pinMode(pin_goosesound, OUTPUT);

  //relays are backwards, high means off
  digitalWrite(pin_lidLight, HIGH);
  digitalWrite(pin_redLight, HIGH);
  digitalWrite(pin_goosesound, HIGH);


  servoLid.attach(pin_servoLid);
  servoArm.attach(pin_servoArm);

  servoArm.write(119);
  delay(500);
  servoLid.write(0);
  delay(500);

  customArray[0] = "DADB";
  customArray[1] = "BDAD";
  customArray[2] = "CCCB";
  customArray[3] = "ABBB";
  customArray[4] = "AACA";
  customArray[5] = "CBDC";
}

//-------------------------------
// MAIN LOOP
//-------------------------------
void loop() {
  if (digitalRead(pin_switch) == HIGH)
  {
    Serial.println ("switch is on");
    if (1){
      digitalWrite(pin_goosesound, LOW);
      delay(500);
      digitalWrite(pin_goosesound, HIGH);
      runCustomSequence();
    }
    else //preset or invalid
    {
      runPresetSequence();
    }
  }
}

//-------------------------------
// HELPER FUNCTIONS
//-------------------------------
void runCustomSequence() {
  //pick an array element from customArray[] at random
  //for each character in that element, run the approproate Action static class function, keep multithreading in mind

  //generate random index number between 0 and size of customArray
  int rando = random(0, customArrayNumItems);
  int index = 0;
  const char * charArray = customArray[rando].c_str(); //convert string in customArray to an array of chars
  char letter;

  Serial.print("Running Custom Sequence: ");
  Serial.println(rando);

  //parse through characters of string in random index
  for(int i = 0; i < MODULES; i++){  //REV.IEW PARSING**//
    //send each character of string to appropriate action function depending on the where the char is in the string
    letter = charArray[i];
    switch (i){
      case 0:
        actuateLid(letter);
        break;
      case 1:
        actuateLidLED(letter);
        break;
      case 2:
        actuateRedLED(letter);
        break;
      case 3:
        actuateArm(letter);
        break;
    }
  }
    m_reset(); //reset all lids and lid positions
}

void runPresetSequence() {
  //randomize a number between 0 and number of preset functions (5)
  //based on randomized number, run appropriate Action static class function (pertaining to presets)

  int rando = random(0, presetArrayNumItems); //random number generator between 0 and size of customArray
  Serial.print("Running Preset Sequence: ");
  Serial.println(rando);

  switch (rando)
  {
    case 0:
      //ToDo: run preset function 0
      break;
    case 1:
      //ToDo: run preset function 1
      break;
    case 2:
      //ToDo: run preset function 2
      break;
  }
}

int jsonParser(String jsonInput)
{
  //JSON format: {"type":1, "data":["AAAA","ABCA","DCAC","ABCD","DCAC","ACDC"]}
  //If type = 0, set typeFlag to 0, return 0.
  //Else, set typeFlag to 1, store data JsonArray in customArray[] dynamically as below, return 1. Else, return -1.
  Serial.printf("Data from Android: %s\n", jsonInput.c_str());

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(const_cast<char*>(jsonInput.c_str()));

  if (!root.success())
  {
    Serial.println("ERROR: Unable to parse input.");
    return -1;
  }

  if (root["type"] == 0)
  {
    Serial.println("The type requested is 'preset' - 0.");
    typeFlag = PRESET;
    return 0;
  }
  else if (root["type"] == 1)
  {
    Serial.println("The type requested is 'custom' - 1.");
    typeFlag = CUSTOM;
    JsonArray& dataArray = root["data"].asArray();
    customArrayNumItems = dataArray.size();

    for (int i = 0; i < customArrayNumItems; i++)
    {
      customArray[i] = dataArray[i];

      //for testing purposes
      Serial.printf("Successfully stored rows from cloud input: %s\n", customArray[i].c_str());
    }

    return 1;
  }
  else
  {
    typeFlag = INVALID;
    Serial.println("ERROR: The 'type' value is invalid.");
    return -1;
  }
}

void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed) {
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

void shakeServo (Servo thisServo) {
  closeLid();//close lid before shaking
  for (int i = 0; i < 10; i++) {//oscillate up and down for 10 cycles
    moveServo(thisServo, 0, 15, 15); //open quickly (to 10 degrees)
    moveServo(thisServo, 15, 0, 15); //close quickly (to 10 degrees)
  }
}

void closeLid(){
  servoLid.write(0);  //set servo angle to 0/close lid
  delay(500);
}

void m_reset(){
  digitalWrite(pin_lidLight, HIGH);
  digitalWrite(pin_redLight, HIGH);
  digitalWrite(pin_goosesound, HIGH);

  servoArm.write(119);
  delay(500);

  closeLid();
}

//-------------------------------
// MAIN FUNCTIONS
//-------------------------------
void actuateLidLED (char letter) {
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
      for (int i = 0; i < 3; i++)
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

void actuateRedLED (char letter) {
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
      for (int i = 0; i < 3; i++)
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

void actuateLid (char letter) {
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

void actuateArm (char letter) {
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
      delay(500);
      moveServo(servoLid, 0, 100, normal);
      break;

    default:
      //do nothing
      break;
  }
}

void actuateGooseSound(char letter) {
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
