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
int presetArrayNumItems = 4;

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
void preset1();
void preset2();
void preset3();
void preset4();

//-------------------------------
// SETUP
//-------------------------------
void setup() {
  Particle.function("jsonParser", jsonParser);
  Particle.function("pre-test", preset_testing);
  Particle.function("cus-test", custom_testing);
  Particle.function("toggleType", toggleType);

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

  servoArm.write(135);
  delay(500);
  servoLid.write(0);
  delay(500);

  typeFlag = PRESET;

  //toggle
  #define DEFAULTCUSTOM
    #ifdef DEFAULTCUSTOM
    customArray[0] = "DADBA";
    customArray[1] = "BDADB";
    customArray[2] = "CCCBA";
    customArray[3] = "ABBBB";
    customArray[4] = "AACAA";
    customArray[5] = "CBDCB";
    #endif
}

//-------------------------------
// MAIN LOOP
//-------------------------------
void loop(){

  if (Serial.available())
  {
    char in[5];
    int i = 0;
    while (Serial.available() > 0  && i <5)
    {
      in[i] = Serial.read();
      Serial.print(in[i]);
      i++;
    }
    Serial.println("");
    runCustomSequence(in[0], in[1], in[2], in[3], in[4]);
  }

  if (digitalRead(pin_switch) == HIGH)
  {
    Serial.println ("switch is on");
    if (typeFlag == CUSTOM){
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
      Serial.println(rando+1);

      runPresetSequence(rando);
    }
  }
}

//-------------------------------
// ACTION FUNCTIONS
//-------------------------------
void runCustomSequence(char act1, char act2, char act3, char act4, char act5) {

  //Schedule order of actions
  bool run_lidled = false;
  bool run_redled = false;

  bool lidledDELAY = false;
  bool lidledFLICKER = false;
  bool redledDELAY = false;
  bool redledFLICKER = false;
  if (act1 == 'B') lidledDELAY = true;
  if (act1 == 'D') lidledFLICKER = true;
  if (act2 == 'B') redledDELAY = true;
  if (act2 == 'D') redledFLICKER = true;

  //1______
  actuateGooseSound(act5);

  if (!lidledDELAY)
  {
    if (act1 != 'C')
    {
      //either on or flicker
      digitalWrite(pin_lidLight, LOW);
    }
  }

  if (!redledDELAY)
  {
    if (act2 != 'C' && !redledFLICKER)
    {
      //on
      digitalWrite(pin_redLight, LOW);
    }
  }

  actuateLid(act3);

  //2______
  if (act1 != 'C')
  {
    actuateLidLED(act1); //flicker, on, or delayed
    if (redledDELAY)
    {
      actuateRedLED(act2); //delay
    }
  }

  actuateArm(act4);
  digitalWrite(pin_lidLight, HIGH);
  delay(600);

  if (redledFLICKER)
  {
    actuateRedLED(act2); //must be flicker
    digitalWrite(pin_redLight, HIGH);
  }

  //3______
    m_reset(); //reset all lids and lid positions
}

void runPresetSequence(int num) {
  switch (num)
  {
    case 0:
      preset1();
      break;
    case 1:
      preset2();
      break;
    case 2:
      preset3();
      break;
    case 3:
      preset4();
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

int preset_testing (String in)
{
  int num = atoi(in);
  runPresetSequence(num-1);
}

int custom_testing (String in)
{
  char c_in[6];
  in.toCharArray(c_in, 6);
  runCustomSequence(c_in[0], c_in[1], c_in[2], c_in[3], c_in[4]);

}

int toggleType(String in)
{
  /*
  if (typeFlag == PRESET)
  {
    typeFlag = CUSTOM;
    return 1;
  }
  else if (typeFlag == CUSTOM)
  {
    typeFlag = PRESET;
    return 0;
  }
  */
  if (in.equals("PRESET"))
  {
    typeFlag = PRESET;
    return 0;
  }
  else if (in.equals("CUSTOM"))
  {
    typeFlag = CUSTOM;
    return 1;
  }

}

//-------------------------------
// Preset Action Functions
//-------------------------------

//only sound, slow open, fast finish
void preset1()
{
  actuateGooseSound('A');
  digitalWrite(pin_lidLight, LOW);
  delay(5500);
  for (int pos = 0; pos <= 50; pos += 1) {
    servoLid.write(pos);
    delay(50);
  }
  digitalWrite(pin_redLight, LOW);
  delay(1000);
  moveServo(servoLid, 50, 100, fast);
  moveServo(servoArm, 135, 0, fast);
  digitalWrite(pin_lidLight, HIGH);
  digitalWrite(pin_redLight, HIGH);
  delay(500);
  m_reset();
}

//fast sequence, then pops up again "im watching you"
void preset2()
{
  digitalWrite(pin_lidLight, LOW);
  actuateLid('B');
  actuateArm('B');
  delay(600);
  digitalWrite(pin_lidLight, HIGH);
  m_reset();
  delay(2000);
  digitalWrite(pin_redLight, LOW);
  for (int pos = 0; pos <= 50; pos += 15) {
    servoLid.write(pos);
    delay(15);
  }
  delay(1000);
  digitalWrite(pin_redLight, HIGH);
  for (int pos = 50; pos >= 0; pos -= 1) {
    servoLid.write(pos);
    delay(70);
  }
}

//fakes you, closes, then quick actual sequence
void preset3()
{
  digitalWrite(pin_lidLight, LOW);
  actuateLid('A');
  moveServo(servoArm, 135, 30, 5);
  delay(1000);
  for (int pos = 30; pos <= 135; pos += 1) {
    servoArm.write(pos);
    delay(30);
  }
  for (int pos = 50; pos >= 0; pos -= 1) {
    servoLid.write(pos);
    delay(50);
  }
  delay(2000);
  actuateLid('B');
  actuateArm('B');
  delay(500);
  digitalWrite(pin_lidLight, HIGH);
  m_reset();
}

//can't close light, gets angry
void preset4()
{
  //arm first
  digitalWrite(pin_lidLight, LOW);
  actuateLid('A');
  moveServo(servoArm, 135, 0, normal);
    digitalWrite(pin_lidLight, HIGH);
    delay(500);
    moveServo(servoArm, 0, 60, normal);
      digitalWrite(pin_lidLight, LOW);
      moveServo(servoArm, 60, 80, slow);
    delay(750);

  //arm second
  moveServo(servoArm, 80, 10, slow);
    digitalWrite(pin_lidLight, HIGH);
    delay(750);
    moveServo(servoArm, 10, 80, slow);
      digitalWrite(pin_lidLight, LOW);
      moveServo(servoArm, 80, 100, slow);
    delay(1000);

  //during lid close
  moveServo(servoArm, 100, 10, fast);
    delay(300);
    digitalWrite(pin_lidLight, HIGH);
    delay(1000);
    moveServo(servoArm, 10, 135, slow);
  moveServo(servoLid, 100, 50, normal);
    digitalWrite(pin_lidLight, LOW);
    moveServo(servoLid, 50, 30, slow);
    delay(750);

  //angry
  digitalWrite(pin_redLight, LOW);
  actuateGooseSound('A');
  delay(2000);
  moveServo(servoLid, 70, 100, fast);

  moveServo(servoArm, 135, 0, fast);
  delay(500);
    for (int i =0; i < 5; i++)
    {
      moveServo(servoArm, 0, 55, fast);
      delay(200);
      moveServo(servoArm, 55, 0, fast);
      delay(200);
    }
  delay(500);
  digitalWrite(pin_lidLight, HIGH);
  delay(1000);
  m_reset();
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
      delay(100);
      break;
    //FAST
    case 'B':
      moveServo(servoLid, 0, 100, fast);
      delay(100);
      break;
    //SLOW
    case 'C':
      moveServo(servoLid, 0, 100, slow);
      delay(100);
      break;
    //SHAKE
    case 'D':
      closeLid(); //make sure lid closed before shaking
      for (int i = 0; i < 10; i++) {//oscillate up and down for 10 cycles
        moveServo(servoLid, 0, 30, 15); //open quickly
        moveServo(servoLid, 30, 0, 15); //close quickly
      }
      moveServo(servoLid, 0, 100, normal);
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
      moveServo(servoArm, 135, 60, normal);
      moveServo(servoArm, 60, 0, fast);
      break;
    //FAST
    case 'B':
      moveServo(servoArm, 135, 0, fast);
      break;
      //SLOW
    case 'C':
      moveServo(servoArm, 135, 60, slow);
      moveServo(servoArm, 60, 0, fast);
      break;
    //SHAKE
    case 'D':
      servoLid.write(100); //make sure lid is open before shaking
      delay(500);
      for (int i = 0; i < 2; i++) { //jiggle 2 cycles
        //moveServo(servoArm, 135, 90, fast); //open quickly (to 10 degrees)
        servoArm.write(60);
        delay(600);
        //moveServo(servoArm, 90, 135, fast); //close quickly (to 10 degrees)
        servoArm.write(135);
        delay(500);
      }
      delay(500);
      moveServo(servoArm, 135, 0, fast);
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
      delay(1500);
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
  if(angleIncr)
  {
    for (pos = startAngle; pos <= endAngle; pos += angSpeed) { // goes from start angle to end angle in steps of angular speed variable
      thisServo.write(pos);           // tell servo to go to position in variable 'pos'
      delay(15);                      // waits 15ms for the servo to reach the position
    }
  }
  //Decreasing servo Angle
  else
  {
    for(pos = startAngle; pos >= endAngle; pos -= angSpeed) { // goes from end angle to start angle in steps of angular speed variable
      thisServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
}

void closeLid(){
  moveServo (servoLid, 100, 0, fast);  //set servo angle to 0/close lid
  delay(500);
}

void m_reset(){
  digitalWrite(pin_lidLight, HIGH);
  digitalWrite(pin_redLight, HIGH);
  digitalWrite(pin_goosesound, HIGH);

  servoArm.write(135);
  delay(500);
  servoLid.write(0);
}
