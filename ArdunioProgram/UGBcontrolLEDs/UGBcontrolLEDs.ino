#include <Thread.h>

//pins
const int pin_ledlidinterrupt = 2;
const int pin_ledredinterrupt = 3;

const int pin_ledlid = 7;
const int pin_ledred = 8;

//threads
Thread thread_ledlid = Thread();
Thread thread_ledred = Thread();

//callback functions
void flickerled_lid()
{
  
}

void flickerled_red()
{
  
}

//setup and loop
void setup()
{
  //pins
  pinMode(pin_ledlid, OUTPUT);
  digitalWrite(pin_ledlid, HIGH);
  
  pinMode(pin_ledred, OUTPUT);
  digitalWrite(pin_ledred, HIGH);

  //threads
  thread_ledlid.onRun(flickerled_lid);
  thread_ledred.onRun(flickerled_red);
  
  //interrupts
  interrupts();  //enable interrupts
  attachInterrupt(digitalPinToInterrupt(pin_ledlidinterrupt),interrupt_ledlid ,RISING);
  attachInterrupt(digitalPinToInterrupt(pin_ledredinterrupt),interrupt_ledred ,RISING);
}

void loop()
{
  //nothing
}

// Interrupts
void interrupt_ledlid()
{
  thread_ledlid.run();
}

void interrupt_ledred()
{
  thread_ledred.run();
}



