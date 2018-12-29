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
//------------------------------------------------------------------------------
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

// MOTOR A
void forwardMotor_A(int speed)
{
  Serial.println(">> | MOTOR-A | FORWARD | FULL STEP | <<");
  digitalWrite(directionStepperA, HIGH);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
}
void reverseMotor_A(int speed)
{
  Serial.println(">> | MOTOR-A | REVERSE | FULL STEP | <<");
  digitalWrite(directionStepperA, LOW);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
}
void forwardQuarterMotor_A(int speed)
{
  Serial.println(">> | MOTOR-A | FORWARD | 1 / 4 STEP | <<");
  sendToRegister(B00100000);
  digitalWrite(directionStepperA, HIGH);
  for(int x = 0; x < 800; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
  sendToRegister(B00000000);
}
void reverseQuarterMotor_A(int speed)
{
  Serial.println(">> | MOTOR-A | REVERSE | 1 / 4 STEP | <<");
  sendToRegister(B00100000);
  digitalWrite(directionStepperA, LOW);
  for(int x = 0; x < 800; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    delayMicroseconds(speed);
  }
  sendToRegister(B00000000);
}

// MOTOR B
void forwardMotor_B(int speed)
{
  Serial.println(">> | MOTOR-B | FORWARD | FULL STEP | <<");
  digitalWrite(directionStepperB, LOW);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void reverseMotor_B(int speed)
{
  Serial.println(">> | MOTOR-B | REVERSE | FULL STEP | <<");
  digitalWrite(directionStepperB, HIGH);
  for(int x = 0; x < 200; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void forwardQuarterMotor_B(int speed)
{
  Serial.println(">> | MOTOR-B | FORWARD | 1 / 4 STEP | <<");
  sendToRegister(B00000010);
  digitalWrite(directionStepperB, LOW);
  for(int x = 0; x < 800; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
  sendToRegister(B00000000);
}
void reverseQuarterMotor_B(int speed)
{
  Serial.println(">> | MOTOR-B | REVERSE | 1 / 4 STEP | <<");
  sendToRegister(B00000010);
  digitalWrite(directionStepperB, HIGH);
  for(int x = 0; x < 800; x++)
  {
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
  sendToRegister(B00000000);
}

// MOTOR A and B
void forwardMotor_AB(int speed)
{
  Serial.println(">> | MOTOR- A & B | FORWARD | FULL STEP | <<");
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
void reverseMotor_AB(int speed)
{
  Serial.println(">> | MOTOR- A & B | REVERSE | FULL STEP | <<");
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
void forwardQuarterMotor_AB(int speed)
{
  Serial.println(">> | MOTOR- A & B | FORWARD | 1 / 4 STEP | <<");
  sendToRegister(B00100010);
  digitalWrite(directionStepperA, HIGH);
  digitalWrite(directionStepperB, LOW);
  for(int x = 0; x < 800; x++)
  {
    digitalWrite(triggerStepperA, HIGH);
    digitalWrite(triggerStepperB, HIGH);
    delayMicroseconds(speed);
    digitalWrite(triggerStepperA, LOW);
    digitalWrite(triggerStepperB, LOW);
    delayMicroseconds(speed);
  }
}
void reverseQuarterMotor_AB(int speed)
{
  Serial.println(">> | MOTOR- A & B | FORWARD | 1 / 4 STEP | <<");
  sendToRegister(B00100010);
  digitalWrite(directionStepperA, LOW);
  digitalWrite(directionStepperB, HIGH);
  for(int x = 0; x < 800; x++)
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

int stepTiming = 700;
  forwardMotor_A(stepTiming);
  flashLED(1, 256);

  forwardMotor_B(stepTiming);
  flashLED(1, 256);

  forwardMotor_AB(stepTiming);
  flashLED(3, 256);

  reverseMotor_A(stepTiming);
  flashLED(1, 256);

  reverseMotor_B(stepTiming);
  flashLED(1, 256);

  reverseMotor_AB(stepTiming);
  flashLED(6, 256);

  forwardQuarterMotor_A(stepTiming);
  flashLED(1, 256);

  forwardQuarterMotor_B(stepTiming);
  flashLED(1, 256);

  forwardQuarterMotor_AB(stepTiming);
  flashLED(3, 256);

  reverseQuarterMotor_A(stepTiming);
  flashLED(1, 256);

  reverseQuarterMotor_B(stepTiming);
  flashLED(1, 256);

  reverseQuarterMotor_AB(stepTiming);
  flashLED(3, 256);
}
