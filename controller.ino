#include "application.h"
#include "ArduinoJson.h"
#include "actions.h"
#include "SparkIntervalTimer.h"
#include <string.h>


//-------------------------------
// VARIABLES
//-------------------------------
enum TYPE { PRESET, CUSTOM, INVALID };
IntervalTimer threadTimer;
TYPE typeFlag;
String customArray[6];
int customArrayNumItems;
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
  Particle.function("jsonParser", jsonParser);
  Serial.begin(9600);

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
  else
  {
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
  //randomize a number between 0 and number of preset functions (5)
  //based on randomized number, run appropriate Action static class function (pertaining to presets)

  int rando = random(0, customArrayNumItems);

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
    case 3:
      //ToDo: run preset function 3
      break;
    case 4:
      //ToDo: run preset function 4
      break;
  }
}

int jsonParser(String jsonInput)
{
  //JSON format: {"type":1, "data":["AAAA","ABCA","DCAC","ABCD","DCAC","ACDC"]}
  //If type = 0, set typeFlag to 0, return 0.
  //Else, set typeFlag to 1, store data JsonArray in customArray[] dynamically as below, return 1. Else, return -1.

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(const_cast<char*>(jsonInput.c_str()));

  Serial.println(root["type"].as<int>());

  if (!root.success())
  {
    Serial.println("ERROR: Unable to parse input.");
    return -1;
  }

  if (root["type"] == 0)
  {
    typeFlag = PRESET;
    return 0;
  }
  else if (root["type"] == 1)
  {
    typeFlag = CUSTOM;
    JsonArray& dataArray = root["data"].asArray();
    customArrayNumItems = dataArray.size();

    for (int i = 0; i < dataArray.size(); i++)
    {
      customArray[i] = dataArray[i];

      //for testing purposes
      Serial.println(customArray[i]);
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
