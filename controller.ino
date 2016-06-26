#include "application.h"
#include "ArduinoJson.h"
#include "actions.h"
#include "SparkIntervalTimer.h"
#include <string.h>


//-------------------------------
// VARIABLES
//-------------------------------
enum TYPE { PRESET, CUSTOM };
IntervalTimer threadTimer;
bool typeFlag = 0;
String customArray[6];
Servo servoLid1, servoLid2, servoArm;
int pos; // variable to store the servo position


//-------------------------------
// MULTITHREADING INTERVALTIMER
//-------------------------------
void threadTimerCB()
{
  //ToDo: call on LED/light functions from new thread
}

//-------------------------------
// SETUP
//-------------------------------
void setup() {
  Serial.begin(9600);
  Particle.function("jsonInput", jsonParser);

  //Attach lid servos to pin 9
  servoLid1.attach(9);
  servoLid2.attach(9);
  //Attach arm servo to pin 10
  servoArm.attach(10);
}

//-------------------------------
// MAIN LOOP
//-------------------------------
void loop() {
  //ToDo: detect switch state (on, off); check if typeFlag is 0 or 1.
  //If 1, call runCustomSequence()
  //If 0, call runPresetSequence()
  if (typeFlag){
    runCustomSequence();
  }
  else{
    runPresetSequence();
  }
}

//-------------------------------
// HELPER FUNCTIONS
//-------------------------------
void runCustomSequence() {
  //pick an array element from customArray[] at random
  //for each character in that element, run the approproate Action static class function, keep multithreading in mind

  //generate random index number between 0 and size of customArray
  int rando = random(0, customArray.size();
  int index = 0;
  //parse through characters of string in random index
  for(std::string::iterator it = customArray[rando].begin(); it != customArray[rando].end(); it++, index++){  //REV.IEW PARSING**//
    //send each character of string to appropriate action function depending on the where the char is in the string
    switch (index){
      case 0:
      actuateLid(*it);
        break;
      case 1:
      actuateLidLED(*it);
        break;
      case 2:
      actuateRedLED(*it);
        break;
      case 3:
      actuateArm(*it);
        break;
    }
  }
}

void runPresetSequence() {
  //randomize a number between 0 and number of preset functions
  //based on randomized number, run appropriate Action static class function (pertaining to presets)
}

int jsonParser(String jsonInput) {
  //JSON format: {"type":1, "data":[AAAA,ABCA,DCAC,AADC,BCAA,AACA,DDAC,ACDC]}
  //If type = 0, set typeFlag to 0, return 0.
  //Else, set typeFlag to 1, store data JsonArray in customArray[] dynamically as below, return 1. Else, return -1.

  /*customArray = new (nothrow) String[size of data jsonArray];
  if (customArray == nullptr) {
  // error assigning memory. Take measures.
  }*/
}
