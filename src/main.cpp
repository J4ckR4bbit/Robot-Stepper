#include <Arduino.h>

//-----------------------------------------------------------------------------
//        CONSTANT VARIABLES
//-----------------------------------------------------------------------------
const byte directionStepperA = 4; // Pin for direction of stepper A
const byte triggerStepperA = 5;// Pin for triggering a step on stepper A
const byte directionStepperB = 6; // Pin for direction of stepper B
const byte triggerStepperB = 7;// Pin for triggering a step on stepper B

const int dataPin = 8; // Pin that sends data to the shift register.
const int latchPin = 9; // Pin that engages the latch on the shift register
const int clockPin = 10; // Pin that for clock signal on the shift register
//-----------------------------------------------------------------------------
//        REGISTER ROUTINES
//-----------------------------------------------------------------------------
void sendToRegister(byte insertByte) // send a byte to shift register
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, insertByte);
  digitalWrite(latchPin, HIGH);
}

void flashLED(int repeats, int periodOne)
{
  for(int x = 0; x < repeats ; x++)
  {
    sendToRegister(B10001000);
    delay(periodOne);
    sendToRegister(B00000000);
    delay(periodOne);
  }
}

//-----------------------------------------------------------------------------
//        MOTOR ROUTINES
//-----------------------------------------------------------------------------
void forwardMotor_A(int speed)
{
  digitalWrite(directionStepperA, HIGH);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
}
void forwardMotor_B(int speed)
{
  digitalWrite(directionStepperB, LOW);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void forwardMotor_AB(int speed)
{
  digitalWrite(directionStepperA, HIGH);
  digitalWrite(directionStepperB, LOW);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void reverseMotor_A(int speed)
{
  digitalWrite(directionStepperA, LOW);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
}
void reverseMotor_B(int speed)
{
  digitalWrite(directionStepperB, HIGH);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void reverseMotor_AB(int speed)
{
  digitalWrite(directionStepperA, LOW);
  digitalWrite(directionStepperB, HIGH);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}


//#############################################################################
//        SEPTUP
//#############################################################################
void setup() {
  //SERIAL COMMUNCATION
  Serial.begin(9600);

  //PIN MODE SETTINGS
  pinMode(directionStepperA, OUTPUT);
  pinMode(triggerStepperA, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);


 // just a simple flashiing of LEDS to indicate set up is complete.
  flashLED(5, 512);

}

//#############################################################################
//        LOOP
//#############################################################################
void loop() {

forwardMotor_A(512);
flashLED(1, 256);

forwardMotor_B(512);
flashLED(1, 256);

forwardMotor_AB(512);
flashLED(3, 256);

reverseMotor_A(512);
flashLED(1, 256);

reverseMotor_B(512);
flashLED(1, 256);

reverseMotor_AB(512);
flashLED(6, 256);

}
