#include "application.h"
#include "config.h"

#include "ArduinoJson.h"
#include "SparkIntervalTimer.h"

#include <string.h>

/* ACTIONS

1. actuateLidLED    |  2. actuateRedLED
 A= on              |   A= on
 B= delayed on      |   B= delayed on
 C= off             |   C= off
 D= flicker         |   D= flicker

3. actuateLid       |  4. actuateArm
 A= normal          |   A= normal
 B= fast            |   B= fast
 C= slow            |   C= slow
 D= shake           |   D= shake

5. actuateGooseSound
 A= On
 B= Off

Note that any modules connected to a relay (i.e. lidLed, redLed, and gooseSound) are all active low.

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

// forward declare
void moveServo (Servo thisServo, int startAngle, int endAngle, int angSpeed);
void closeLid();
void m_reset();
void actuateLid(char letter);
void actuateLidLED(char letter);
void actuateArm(char letter);
void actuateRedLED(char letter);
void actuateGooseSound (char letter);
void runCustomSequence(char act1, char act2, char act3, char act4, char act5);
void runPresetSequence(int num);

//-------------------------------
// SETUP
//-------------------------------
void setup() {
  Particle.function("jsonParser", jsonParser);
  Particle.function("tester", testing);
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

  //toggle
  #define DEFAULTCUSTOM
    #ifdef DEFAULTCUSTOM
    customArray[0] = "DADB";
    customArray[1] = "BDAD";
    customArray[2] = "CCCB";
    customArray[3] = "ABBB";
    customArray[4] = "AACA";
    customArray[5] = "CBDC";
    #endif
}

//-------------------------------
// MAIN LOOP
//-------------------------------
void loop() {
  if (digitalRead(pin_switch) == HIGH)
  {
    Serial.println ("switch is on");
    if (1){
      //generate random index number between 0 and size of customArray
      int rando = random(0, customArrayNumItems);
      const char *charArray = customArray[rando].c_str(); //convert string in customArray to an array of chars
      char act1 = charArray[0];
      char act2 = charArray[1];
      char act3 = charArray[2];
      char act4 = charArray[3];
      char act5 = charArray[4];

      Serial.print("Running Custom Sequence: ");
      Serial.println(rando);

      runCustomSequence(act1, act2, act3, act4, act5);
    }
    else //preset or invalid
    {
      //randomize a number between 0 and number of preset functions
      int rando = random(0, presetArrayNumItems); //random number generator between 0 and size of customArray
      Serial.print("Running Preset Sequence: ");
      Serial.println(rando);

      runPresetSequence(rando);
    }
  }
}

//-------------------------------
// ACTION FUNCTIONS
//-------------------------------
void runCustomSequence(char act1, char act2, char act3, char act4, char act5) {

  //Schedule order of actions
  bool lidledDELAY = false;
  bool lidledFLICKER = false;
  bool redledDELAY = false;
  bool redledFLICKER = false;
  if (act1 == 'B') lidledDELAY = true;
  if (act2 == 'B') redledDELAY = true;
  if (act2 == 'D') redledFLICKER = true;

  //1______
  actuateGooseSound(act5);
  if (!lidledDELAY) digitalWrite(pin_lidLight, LOW);
  if (!redledDELAY) digitalWrite(pin_redLight, LOW);

  actuateLid(act3);

  //2______
  if (lidledDELAY) actuateLidLED(act1);
  if (redledDELAY) actuateRedLED(act2);
  actuateArm(act4);
  if (redledFLICKER) actuateRedLED(act2);

  //3______
    m_reset(); //reset all lids and lid positions
}

void runPresetSequence(int num) {
  switch (num)
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

int testing (String in)
{

}

//-------------------------------
// MAIN FUNCTIONS
//-------------------------------
void actuateLidLED (char letter) {
  switch (letter)
  {
    //ON
    case 'A':
      digitalWrite(pin_lidLight, LOW);
      break;
    //DELAYED ON
    case 'B':
      delay(delayed_response_timems);
      digitalWrite(pin_lidLight, LOW);
      break;
    //OFF
    case 'C':
      digitalWrite(pin_lidLight, HIGH);
      break;
    //FLICKER
    case 'D':
    {
      int array[30] = {100,5, 100,5, 60,5, 40,5, 10,5,
                     5,5, 10,5, 5,5, 300,10, 10,10,
                     40,5, 10,30, 40,20, 100,10, 50,5};

      for (int i = 0; i < 15; i=i+2)
      {
        digitalWrite(pin_lidLight, LOW);
        delay(array[i]*10);
        digitalWrite(pin_lidLight, HIGH);
        delay(array[i+1]*10);
      }
      digitalWrite(pin_lidLight, LOW);
      break;
    }
    default:
      //do nothing
      break;
  }
}

void actuateRedLED (char letter) {
  switch (letter)
  {
    //ON
    case 'A':
      digitalWrite(pin_redLight, LOW);
      break;
    //DELAYED ON
    case 'B':
      delay(delayed_response_timems);
      digitalWrite(pin_redLight, LOW);
      break;
    //OFF
    case 'C':
      digitalWrite(pin_redLight, HIGH);
      break;
    //FLICKER
    case 'D':
    {
    int array[20] = {100, 50, 90, 50, 80, 50, 70, 50, 60, 30,
                 20, 20, 20, 20, 20, 15, 15, 15, 15, 15 };

      for (int i = 0; i < 10; i=i+2)
      {
        digitalWrite(pin_redLight, LOW);
        delay(array[i]*10);
        digitalWrite(pin_redLight, HIGH);
        delay(array[i+1]);
      }
      digitalWrite(pin_redLight, LOW);
      break;
    }
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
      moveServo(servoLid, 0, 100, normal);
      break;
    //FAST
    case 'B':
      moveServo(servoLid, 0, 100, fast);
      break;
    //SLOW
    case 'C':
      moveServo(servoLid, 0, 100, slow);
      break;
    //SHAKE
    case 'D':
      closeLid(); //make sure lid closed before shaking
      for (int i = 0; i < 10; i++) {//oscillate up and down for 10 cycles
        moveServo(servoLid, 0, 15, 15); //open quickly (to 10 degrees)
        moveServo(servoLid, 15, 0, 15); //close quickly (to 10 degrees)
      }
      moveServo(servoLid, 0, 50, slow);
      delay(500);
      moveServo(servoLid, 0, 100, slow);
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
      moveServo(servoArm, 119, 0, normal);
      break;
    //FAST
    case 'B':
      moveServo(servoArm, 119, 0, fast);
      break;
      //SLOW
    case 'C':
      moveServo(servoArm, 119, 0, slow);
      break;
    //SHAKE
    case 'D':
      servoLid.write(100); //make sure lid is open before shaking
      delay(500);
      for (int i = 0; i < 2; i++) { //jiggle 2 cycles
        moveServo(servoArm, 0, 90, 15); //open quickly (to 10 degrees)
        moveServo(servoArm, 90, 0, 15); //close quickly (to 10 degrees)
      }
      delay(500);
      moveServo(servoArm, 119, 0, normal);
      break;

    default:
      //do nothing
      break;
  }
}

void actuateGooseSound(char letter) {
  switch (letter)
  {
    //ON
    case 'A':
      digitalWrite(pin_goosesound, LOW);
      delay(100);
      digitalWrite(pin_goosesound, HIGH);
      break;
    //OFF
    case 'B':
      //do nothing
      break;

    default:
      //do nothing
      break;
  }
}

//-------------------------------
// HELPER FUNCTIONS
//-------------------------------

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
