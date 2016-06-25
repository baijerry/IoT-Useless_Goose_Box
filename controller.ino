#include "application.h"
#include "ArduinoJson.h"
#include "SparkIntervalTimer.h"

//-------------------------------
// VARIABLES
//-------------------------------
enum TYPE { PRESET, CUSTOM };
IntervalTimer threadTimer;
bool typeFlag = 0;
String* customArray;

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
}

//-------------------------------
// MAIN LOOP
//-------------------------------
void loop() {
  //ToDo: detect switch state (on, off); check if typeFlag is 0 or 1.
  //If 1, call runCustomSequence()
  //If 0, call runPresetSequence()
}

//-------------------------------
// HELPER FUNCTIONS
//-------------------------------
void runCustomSequence() {
  //pick an array element from customArray[] at random
  //for each character in that element, run the approproate Action static class function, keep multithreading in mind
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

  //this is my first try wohooooo :)))))))
}
