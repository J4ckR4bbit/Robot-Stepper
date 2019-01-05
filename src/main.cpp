


//------------------------------------------------------------------------------
//  LIBRARIES
//------------------------------------------------------------------------------
#include <Arduino.h>
#include <Servo.h>
// #include <HCSR04.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
//------------------------------------------------------------------------------
//        WEMOS PINS
//------------------------------------------------------------------------------
const int rightServoPin = 16;
const int leftServoPin = 14;
const int frontServoPin = 12;

const int triggerPin = 15;
const int echoPin = 2;

//------------------------------------------------------------------------------
//  LIB SETTINGS
//------------------------------------------------------------------------------
Servo servoFront;
Servo servoRight;
Servo servoLeft;

//UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);  // Initialize sensor that uses digital pins 2[9](ECHO) and 15[10] (TRIGGER)
LiquidCrystal_PCF8574 lcd(0x3F);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//------------------------------------------------------------------------------
//        SETTING VARIABLES
//------------------------------------------------------------------------------
int serialMode = 1; // If 1 then serial printing will occur if 0 then it wont.
int lcdMode = 1;
//------------------------------------------------------------------------------
//        CONSTANT VARIABLES
//------------------------------------------------------------------------------
//for sensor servo
const int posMaxRGT = 30;
const int posHalfRGT = 60;

const int posCenter = 90; // position of servoFront

const int posHalfLFT = 120;
const int posMaxLFT = 150;

// for wheel servos
const int noSpeed = 90;
const int rightFrwdMaxSpeed = 0;
const int rightRvrsMaxSpeed = 180;
const int leftFrwdMaxSpeed = 180;
const int leftRvrsMaxSpeed = 0;
//------------------------------------------------------------------------------
//  COUNTER
//------------------------------------------------------------------------------
unsigned long loopCounter = 0;
//------------------------------------------------------------------------------
//  VARIABLES
//------------------------------------------------------------------------------
int nowPos;        // the angle of the ultrasonic sensor changes in for loop.

// DISTANCE HC_SR04
float distFloat; // value read directly from the UltraSonic Sensor.

float distMeters;
float distCentiMeters;
float distMilliMeters;
int distM_int;
int distCM_int;
int distMM_int;
int distCentiVal;
int distMilliVal;

int val_A_OUT;
int val_B_OUT;
int val_C_OUT;
int val_D_OUT;

//TIME
int fullSec = 1024;
int halfSec = 512;
int quarterSec = 256;
int eighthSec = 128;

// liquid Crystal display
int show;
//##############################################################################
//  FUNCTIONS
//##############################################################################

//..............................................................................
//   TIME
//..............................................................................
void delayFor(int delayTime) {
  // unsigned long startDelay = millis();
  // while (millis() - startDelay <= delayTime )
  // {
  //   if (millis() - startDelay >= delayTime )
  //   {
  //     break;
  //   }
  // }
  delay(delayTime);
}
//..............................................................................
//  SERIAL BASICS
//..............................................................................
void printLine() {
  if (serialMode == 1) {
    Serial.print("-------------------------------------------\n");
  }
}
void printDashLine() {
  if (serialMode == 1) {
    Serial.print("- - - - - - - - - - - - - - - - - - - - - -\n");
  }
}
void printDotLine() {
  if (serialMode == 1) {
    Serial.print(". . . . . . . . . . . . . . . . . . . . . .\n");
  }
}
void printHashLine() {
  if (serialMode == 1) {
    Serial.print("###########################################\n");
  }
}
void printBlank() {
  if (serialMode == 1) {
    Serial.print("\n");
  }
}
void printError() {
  if (serialMode == 1)
  {
    Serial.print("|||||||||||||||||| ERROR ||||||||||||||||||");
    printBlank();
  }
}
void printReady() {
  if (serialMode == 1)
  {
    Serial.print("<<<<<<<<<<<<<<<<< READY >>>>>>>>>>>>>>>>>>");
    printBlank();
  }
}
//..............................................................................
//  PRINT INFO TO SERIAL
//..............................................................................
void printObjectInfo(String objectType, String fieldOne, String fieldTwo, String symbols) {
  if (serialMode == 1)  {
    printLine();
    Serial.print(symbols);
    Serial.print("|  ");
    Serial.print(objectType);
    Serial.print("\t|  ");
    Serial.print(fieldOne);
    Serial.print("  |  ");
    Serial.print(fieldTwo);
    Serial.print("\t|");
    Serial.print(symbols);
    Serial.print("\n");
  }
}
void printSensorInfo(String objectType, String fieldOne, String fieldTwo) {
  if (serialMode == 1)
  {
    printDashLine();
    Serial.print("??|  ");
    Serial.print(objectType);
    Serial.print("\t|  ");
    Serial.print(fieldOne);
    Serial.print("  |  ");
    Serial.print(fieldTwo);
    Serial.print("\t|??\n");
  }
}
void printReturnBasic(String variableOne, int returnValue) {
  if (serialMode == 1) {
    Serial.print("\t[ ");
    Serial.print(variableOne);
    Serial.print(" | ");
    Serial.print(returnValue);
    Serial.print(" ]\n");
  }
}
void printReturnBasic_OUT(String variableOne, int returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t\t|| ");
    Serial.print(variableOne);
    Serial.print(" | ");
    Serial.print(returnValue);
    Serial.print(" ||\n");
  }
}
void printReturnString(String variableOne) {
  if (serialMode == 1)  {
    Serial.print("\t[ ");
    Serial.print(variableOne);
    Serial.print(" ]\n");
  }
}
void printReturnTwoString(String variableOne, String variableTwo) {
  if (serialMode == 1)  {
    Serial.print("\t[ ");
    Serial.print(variableOne);
    Serial.print(" | ");
    Serial.print(variableTwo);
    Serial.print(" ]\n");
  }// takes two words and prints it in [w|w]
}
void printReturnToggle(String variableOne, int returnValue, int newLine) {
  if (serialMode == 1)  {
    Serial.print("\t[ ");
    Serial.print(variableOne);
    Serial.print(" : ");
    Serial.print(returnValue);
    if (newLine == 1) // goes to the next line if newline is 1.
    {
      Serial.print(" ]\n");
    }
    if (newLine == 0)
    {
      Serial.print(" ] ");
    }
  }
}
void printReturnOne_B(String variableOne, int returnValue ) {
  if (serialMode == 1) {
    Serial.print(" ( ");
    Serial.print(variableOne);
    Serial.print(" : ");
    Serial.print(returnValue);
    Serial.print(" )\n");
  } // takes a word and a interger and prints it in ( w | i ) has no tab!!
}
void printReturnTwo(String variableOne, int returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t( ");
    Serial.print(variableOne);
    Serial.print(" : ");
    Serial.print(returnValue);
    Serial.print(" )\n");
  } // takes a word and a interger and prints it in ( w | i )
}
void printReturnThree(String variableOne, int returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t{ ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue);
    Serial.print(" }\n");
  } // takes a word and a interger and prints it in { w | i }
}
void printReturnFour(String variableOne, int returnValue) {
  if (serialMode == 1)
  {
    Serial.print("\t< ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue);
    Serial.print(" >\n");
  } // takes a word and a interger and prints it in { w | i }
}
void printReturnFloat(String variableOne, float returnValue) {
  if (serialMode == 1)
  {
    Serial.print("\t{ ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue, 1);
    Serial.print(" }\n");
  }
}
void printReturnSec(String variableOne, float returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t! ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue);
    Serial.print("s !\n");
  }
}
void printReturnMilliSec(String variableOne, float returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t! ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue);
    Serial.print("ms !\n");
  }
}
void printReturnMircoSec(String variableOne, unsigned long returnValue) {
  if (serialMode == 1)  {
    Serial.print("\t@ ");
    Serial.print(variableOne);
    Serial.print(" = ");
    Serial.print(returnValue);
    Serial.print("us @\n");
  }
}

//..............................................................................
//  SERVO
//..............................................................................
void sensorServo(int movePos) {
  servoFront.write(movePos);
}// takes an angle  0 - 180 and moves the servo.
void turnSensorServo(int fromPos, int toPos, int delayPWM) {
  int deltaPos = toPos - fromPos; // checks the difference between initial and final positions

  if (deltaPos > 0) // activated when deltaPos is positive
  {
    for (nowPos = fromPos; nowPos <= toPos; nowPos++)// in steps of 1 degree
    {
      sensorServo(nowPos);
      delayFor(delayPWM);
    } //for (pos++)
  }// if (deltaPos > 0)

  if (deltaPos < 0) // activated when deltaPos is negative
  {
    for (nowPos = fromPos; nowPos >= toPos; nowPos--)// in steps of 1 degree
    {
      sensorServo(nowPos);
      delayFor(delayPWM);
    } //for (pos--)
  }// if (deltaPos < 0)

  if (deltaPos == 0) // activated when deltaPos is negative
  {
    sensorServo(nowPos);
    delayFor(fullSec);
  }// if (deltaPos == 0)
  //---------------------------------------------------------

  if (serialMode == 1)
  {
    printObjectInfo("SERVO", "FRONT", "SG-90", "++");

    Serial.print("\t[  from = ");//start new variable print
    if (fromPos == 90) // print "MID"
    {
      Serial.print("MID");
    } // if (mid)
    if (fromPos > 90)// print "LFT"
    {
      Serial.print("LFT");
    }// if (lft)
    if (fromPos < 90)// print "RGT"
    {
      Serial.print("RGT");
    }// if (rgt)
    Serial.print(" ] ");

    printReturnOne_B("angle", fromPos);

    Serial.print("\t[    to = "); //start new variable print
    if (toPos == 90)
    {
      Serial.print("MID");
    }// if (mid)
    if (toPos > 90)
    {
      Serial.print("LFT");
    }// if (lft)
    if (toPos < 90)
    {
      Serial.print("RGT");
    }// if (rgt)
    Serial.print(" ] "); // end line of serial print
    printReturnOne_B("angle", toPos);
    printReturnThree("delta", deltaPos);
    printReturnMilliSec("delay", delayPWM);
    printBlank();
  }
}// move the servo on the front.
//..............................................................................
//  MOVEMENT
//..............................................................................
//const int rightFrwdMaxSpeed = 0;
//const int rightRvrsMaxSpeed = 180;

void rightWheelGo(int rightSpeed, int goRightFor ) {
  servoRight.write(rightSpeed);
  //---------------------------------------------------------
  if (lcdMode == 1) {
    if (rightSpeed >= rightFrwdMaxSpeed && rightSpeed < 90 )
    {
      lcd.setCursor(12, 1);
      lcd.write("2^");
    }
    if (rightSpeed <= rightRvrsMaxSpeed && rightSpeed > 90 )
    {
      lcd.setCursor(12, 1);
      lcd.write("1v");
    }
  }

  float inSecR = (float)goRightFor / 1000.00; // turns the millis into seconds
  printObjectInfo("SERVO", "RIGHT", "FS-90R", ">>");
  printReturnBasic("speed", rightSpeed);
  printReturnFloat("move", inSecR);
  printBlank();

  delayFor(goRightFor);
}
//const int leftFrwdMaxSpeed = 180;
//const int leftRvrsMaxSpeed = 0;
void leftWheelGo(int leftSpeed, int goLeftFor ) {
  servoLeft.write(leftSpeed);
  //---------------------------------------------------------
  if (lcdMode == 1) {
    if (leftSpeed <= leftFrwdMaxSpeed && leftSpeed > 90 )
    {
      lcd.setCursor(1, 1);
      lcd.write("^2");
    }
    if (leftSpeed >= leftRvrsMaxSpeed && leftSpeed < 90 ) {
      lcd.setCursor(1, 1);
      lcd.write("v1");
    }
  }

  float inSecL = (float)goLeftFor / 1000.00;// turns the millis into seconds
  printObjectInfo("SERVO", "LEFT", "FS-90R", "<<");
  printReturnBasic("speed", leftSpeed);
  printReturnFloat("move", inSecL);
  printBlank();

  delayFor(goLeftFor);
}
void bothWheelsGo(int rightSpeed, int leftSpeed, int goBothFor) {
  servoRight.write(rightSpeed);
  servoLeft.write(leftSpeed);

  //---------------------------------------------------------
  if (lcdMode == 1) {

    if (rightSpeed >= rightFrwdMaxSpeed && rightSpeed < 90 )
    {
      lcd.setCursor(12, 1);
      lcd.write("2^");
    }
    if (rightSpeed <= rightRvrsMaxSpeed && rightSpeed > 90 )
    {
      lcd.setCursor(12, 1);
      lcd.write("1v");
    }

    if (leftSpeed <= leftFrwdMaxSpeed && leftSpeed > 90 )
    {
      lcd.setCursor(1, 1);
      lcd.write("^2");
    }
    if (leftSpeed >= leftRvrsMaxSpeed && leftSpeed < 90 ) {
      lcd.setCursor(1, 1);
      lcd.write("v1");
    }
  }

  float inSecB = (float)goBothFor / 1000.00;// turns the millis into seconds

  printObjectInfo("SERVO", "BOTH", "FS-90R", "<>");

  printReturnTwoString("RIGHT ", "WHEEL");
  printReturnBasic("speed", rightSpeed);
  printReturnFloat("time", inSecB);

  printReturnTwoString("LEFT ", "WHEEL");
  printReturnBasic("speed", leftSpeed);
  printReturnFloat("time", inSecB);
  printBlank();

  delayFor(goBothFor);
}
void rightWheelStop(int stopRightFor) {
  servoRight.write(noSpeed);
  //---------------------------------------------------------
  if (lcdMode = 1) {
    lcd.setCursor(12, 1);
    lcd.write("0-");
  }

  printObjectInfo("SERVO", "RIGHT", "FS-90R", "#>");
  printReturnBasic("speed", 0);
  printBlank();
  delayFor(stopRightFor);

}
void leftWheelStop(int stopLeftFor) {
  servoLeft.write(noSpeed);
  //---------------------------------------------------------
  if (lcdMode = 1) {
    lcd.setCursor(1, 1);
    lcd.write("-0");
  }

  printObjectInfo("SERVO", "LEFT", "FS-90R", "<#");
  printReturnBasic("speed", 0);
  printBlank();
  delayFor(stopLeftFor);
}
void bothWheelsStop(int stopBothFor) {
  servoRight.write(noSpeed);
  servoLeft.write(noSpeed);
  //---------------------------------------------------------
  if (lcdMode = 1)
  {
    lcd.setCursor(1, 1);
    lcd.write("-0");
    lcd.setCursor(12, 1);
    lcd.write("0-");
  }

  printObjectInfo("SERVO", "BOTH", "FS-90R", "##");
  printReturnBasic(" left", 0);
  printReturnBasic("right", 0);
  printBlank();

  delayFor(stopBothFor);

}
//..............................................................................
//  SENSORS
//..............................................................................
void getDistance() {
  //  distFloat = distanceSensor.measureDistanceCm();
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  unsigned long durationMicroSec = pulseIn(echoPin, HIGH, 8000);
  double distanceCm = durationMicroSec / 2.0 * 0.0343;


  if (distanceCm == 0 || distanceCm > 100) {
    distFloat = -1;
  }
  else {
    distFloat = distanceCm;
  }

  printReturnMircoSec("sensor", durationMicroSec);
}
void getPrintDistance(int delayDistance) {

  printSensorInfo("SENSOR", "HC-SRO4", "FRONT");

  getDistance();

  printReturnFloat("dist", distFloat);
  printBlank();

  delayFor(delayDistance);
}
void sensorDistInt() {
  // getPrintDistance(halfSec);
  // delay(2000);

  printObjectInfo("CALC", "DISTANCE", "CONVERSION", "%%");
  getDistance();
  printReturnFloat("distFloat", distFloat);

  distMilliMeters = distFloat * 100.00;
  printReturnFloat("distMilliMeters", distMilliMeters);

  distMM_int = (int)distMilliMeters;
  printReturnBasic("distMM_int", distMM_int);

  int val_A_IN;
  int val_A_1;
  int val_A_2;
  int val_A_3;


  int val_B_IN;
  int val_B_1;
  int val_B_2;
  int val_B_3;


  int val_C_IN;
  int val_C_1;
  int val_C_2;
  int val_C_3;


  int val_D_IN;
  int val_D_1;
  int val_D_2;





  val_A_IN = distMM_int;
  //...........................................................................

  val_A_1 = val_A_IN;
  printReturnBasic("val_A_1", val_A_1);
  printReturnTwo("val_A_2", val_A_2);

  val_A_2 = val_A_1 / 1000; // this gives a single digit.

  val_A_OUT = val_A_2; // used at the end of sum to print
  printReturnBasic_OUT("val_A_OUT", val_A_OUT);

  val_A_3 = val_A_2 * 1000; // used in the unit below as a "clean Number"
  printReturnThree("val_A_3", val_A_3);
  //...........................................................................
  val_B_IN = val_A_3;
  val_B_1 = (val_A_IN - val_B_IN); // gets rid of the number after digit
  printReturnTwo("val_B_1", val_B_1);

  val_B_2 = val_B_1 / 100; // this gives a single digit.
  printReturnThree("val_B_2", val_B_2);

  val_B_OUT = val_B_2;  // used at the end of sum to print
  printReturnBasic_OUT("val_B_OUT", val_B_OUT);

  val_B_3 = val_B_2 * 100;// used in the unit below as a "clean Number"
  printReturnFour("val_B_Z", val_B_3);
  //...........................................................................

  val_C_IN = val_A_3 + val_B_3;
  val_C_1 = (val_A_IN - val_C_IN); // gets rid of the number after digit
  printReturnTwo("val_C_1", val_C_1);

  val_C_2 = val_C_1 / 10; // this gives a single digit.
  printReturnThree("val_C_2", val_C_2);

  val_C_OUT = val_C_2;  // used at the end of sum to print
  printReturnBasic_OUT("val_C_OUT", val_C_OUT);

  val_C_3 = val_C_2 * 10;// used in the unit below as a "clean Number"
  printReturnFour("val_C_3", val_C_3);

  //...........................................................................

  val_D_IN = val_A_3 + val_B_3 + val_C_3;
  val_D_1 = (val_A_IN - val_D_IN); // gets rid of the number after digit
  printReturnTwo("val_D_1", val_D_1);

  val_D_2 = val_D_1 / 1;
  printReturnThree("val_D_2", val_D_2);

  val_D_OUT = val_D_1;  // used at the end of sum to print
  printReturnBasic_OUT("val_D_OUT", val_D_OUT);



  if (lcdMode == 1)
  {
    int int_Array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char char_Array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    int lcdCountA = 0;
    int lcdCountB = 0;
    int lcdCountC = 0;
    int lcdCountD =  0;
    for (int x = 0; x < 10; x++)
    {
      if (val_A_OUT == int_Array[lcdCountA])
      {
        lcd.setCursor(6, 0);
        lcd.write(char_Array[lcdCountA]);
      }
      lcdCountA++;
      if (val_B_OUT == int_Array[lcdCountB])
      {
        lcd.setCursor(7, 0);
        lcd.write(char_Array[lcdCountB]);
      }
      lcdCountB++;
      if (val_C_OUT == int_Array[lcdCountC])
      {
        lcd.setCursor(8, 0);
        lcd.write(".");
        lcd.write(char_Array[lcdCountC]);

      }
      lcdCountC++;
      if (val_D_OUT == int_Array[lcdCountD])
      {
        lcd.setCursor(10, 0);
        lcd.write(char_Array[lcdCountD]);
      }
      lcdCountD++;
    }


    lcd.setCursor(8, 0);
    lcd.write(".");

  }

  printDotLine();
}


void lcdBasicMenu(int setRowOne, int setRowTwo) {


  int firstCol = 0;
  int secondCol = 0;

  int col = 0;
  int row = 0;

  // ---- ---- FIRST LINE ---- ----
  if (setRowOne == 0) {
    char firstRow[] =  "                ";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(firstRow[firstCol++]);
    }
  }//blank
  if (setRowOne == 1) {
    char firstRow[] =  "# | | --TURN-- #";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(firstRow[firstCol++]);
    }
  }// turn...
  if (setRowOne == 2) {
    char firstRow[] =  "----------------";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(firstRow[firstCol++]);
    }
  }// dashed line
  if (setRowOne == 3) {
    char firstRow[] =  ">   ERROR ON    ";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(firstRow[firstCol++]);
    }
  }
  if (setRowOne == 4) {
    char firstRow[] =  "|RUN|          |";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(firstRow[firstCol++]);
    }
  }



  // ---- ---- SECOND LINE ---- ----
  col = 0;
  row = 1;

  if (setRowTwo == 0) {
    char secondRow[] = "                ";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(secondRow[secondCol++]);
    }
  }
  if (setRowTwo == 1) {
    char secondRow[] =  "# V V -DIP ON- #";
    for (int x = 0; x < 16; x++)
    {
      lcd.setCursor(col++, row );
      lcd.write(secondRow[secondCol++]);
    }
  }
  if (setRowTwo == 2) {
    char secondRow[] = "----------------";
    for (int x = 0; x < 16; x++)
    {
      lcd.setCursor(col++, row );
      lcd.write(secondRow[secondCol++]);
    }
  }
  if (setRowTwo == 4) {
    char secondRow[] = "|   |      |   |";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(secondRow[secondCol++]);
    }
  }
  if (setRowTwo == 3) {
    char secondRow[] = "|LFT|MIDDLE|RGT|";
    for (int x = 0; x < 16; x++) {
      lcd.setCursor(col++, row );
      lcd.write(secondRow[secondCol++]);
    }
  }
}//Basic Print Out to the LCD
//------------------------------------------------------------------------------
//  CHECKS
//------------------------------------------------------------------------------
void initServos() {
  servoFront.write(posCenter);
  servoRight.write(noSpeed);
  servoLeft.write(noSpeed);
}
//------------------------------------------------------------------------------
//  CHECKS
//------------------------------------------------------------------------------
void checkLCD() {
  printBlank();
  printHashLine();
  printReturnTwoString("TESTING", "ULTRASONIC");
  printBlank();
  int error;

  printObjectInfo("SCREEN", "LCD", "16x2", "&&");
  printReturnString("Checking if LCD is ready");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x3F);
  error = Wire.endTransmission();

  if (error == 0)  {
    printReady();
    printReturnString("LCD | FOUND");
    show = 0;
  }
  else  {
    printError();
    printReturnString("LCD | NOT FOUND");
  } // if
  printBlank();

  lcd.begin(16, 2); // initialize the lcd

  if (show == 0)  {
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    lcd.print("----NITA---------");
    lcd.setCursor(0, 1);
    lcd.print("----ROBOT--------");
    delay(1000);
  }

}
void checkUltraSonic() {

  printHashLine();
  printReturnTwoString("TESTING", "ULTRASONIC");
  printBlank();
  getDistance();


  if (distFloat == -1) // checks if the dip switches have been turned on.
  {
    printError();
    printObjectInfo("SENSOR", "HC-SRO4", "FRONT", "??");
    Serial.println("  | switch DIP 1 and 2 - ON  |  ");
    Serial.print("  |");

    int countCheck = 0; // used in while loop to skip to next line.

    while (distFloat == -1) // keep a loop going while the DIP buttons ore off.
    {

      countCheck++;
      Serial.print(".");

      if (countCheck == 26) // sets counter back to zero and start a new serial line.
      {
        Serial.println("|");
        Serial.print("  |");
        countCheck = 0;
      }
      if (countCheck % 2 == 0) {
        lcdBasicMenu(1, 1);
        delayFor(fullSec);
      }
      else {
        lcdBasicMenu(2, 2);
        delayFor(quarterSec);
      }
      getDistance();

      if (distFloat != -1 ) // when dip buttons are on if will be entered breaking the loop. ( -1 is used as that is the returned value if the sensor is not working)
      {
        delayFor(halfSec);
        Serial.println("");
        Serial.println("         -->  ERROR | SORTED |  ");
        Serial.println("");
        delayFor(halfSec);
        break;
      }
    }
  }
  getDistance();
  if (distFloat != -1)
  {
    printReady();
  }
}
void checkServos() {

  getPrintDistance(halfSec);

  turnSensorServo(posCenter,  posMaxLFT, 10);
  getPrintDistance(halfSec);

  turnSensorServo(posMaxLFT,  posCenter, 5);
  getPrintDistance(halfSec);

  turnSensorServo(posCenter,  posMaxRGT, 10);
  getPrintDistance(halfSec);

  turnSensorServo(posMaxRGT,  posCenter, 5);
  getPrintDistance(halfSec);

  rightWheelGo(rightFrwdMaxSpeed, halfSec);
  rightWheelStop(halfSec);
  delayFor(eighthSec);

  leftWheelGo(leftFrwdMaxSpeed, halfSec);
  leftWheelStop(halfSec);
  delayFor(eighthSec);

  bothWheelsGo(rightFrwdMaxSpeed, leftRvrsMaxSpeed, halfSec);
  bothWheelsStop(halfSec);
  delayFor(halfSec);

  bothWheelsGo(rightRvrsMaxSpeed, leftFrwdMaxSpeed, halfSec);
  bothWheelsStop(halfSec);
  delayFor(halfSec);
}
//------------------------------------------------------------------------------
//  START UP
//------------------------------------------------------------------------------
void startUpRoutine() //main routine in setup contains testing and checking
{

  //print "loading" out
  printBlank();
  printBlank();
  printBlank();

  Serial.println("|\t|\t|\t|\t|\t|\t|");
  char loading[] = "loading...";
  int letterInArray = 0;
  Serial.print("\t\t");
  for (int x = 0; x < 10; x++)
  {
    Serial.print(loading[letterInArray]);
    delayFor(eighthSec);
    letterInArray++;
  }

  Serial.println("");
  Serial.println("|\t|\t|\t|\t|\t|\t|");
  // printObjectInfo("OBJECT", "InfoA", "InfoB");
  printHashLine();
  delayFor(halfSec);
  Serial.print("\t    N");
  delayFor(eighthSec);
  Serial.print(" I");
  delayFor(eighthSec);
  Serial.print(" T");
  delayFor(eighthSec);
  Serial.println(" A");
  delayFor(quarterSec);
  Serial.print("\t     R");
  delayFor(quarterSec);
  Serial.print("O");
  delayFor(quarterSec);
  Serial.print("B");
  delayFor(quarterSec);
  Serial.print("O");
  delayFor(quarterSec);
  Serial.print("T      \n");
  delayFor(halfSec);
  printHashLine();
  delayFor(quarterSec);
  Serial.println("  >           ver.1          <  ");
  delayFor(quarterSec);
  printLine();
  Serial.println("@@\t\t  SYSTEM CHECK\t\t @@  ");
  printLine();
  Serial.print("\n");


}
//------------------------------------------------------------------------------
//  SETUP
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  while (! Serial);
  //  SERVOS
  servoFront.attach(frontServoPin);
  servoRight.attach(rightServoPin); // 0 <- forward 90 -> reverse ->180
  servoLeft.attach(leftServoPin); // 0 <-reverse <- 90 -> forward -> 180
  // ULTRA SONIC
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //ULTRA SONIC



  Serial.println("");


  initServos();
  startUpRoutine();
  checkLCD();
  checkUltraSonic();
  lcdBasicMenu(0, 0);

  lcdBasicMenu(2, 2);
}

//------------------------------------------------------------------------------
//  LOOP
//------------------------------------------------------------------------------
void loop()
{


  lcdBasicMenu(4, 4);


  //const int rightFrwdMaxSpeed = 0;
  //const int rightRvrsMaxSpeed = 180;
  //const int leftFrwdMaxSpeed = 180;
  //const int leftRvrsMaxSpeed = 0;

  int lowSpeed = 30;
  int highSpeed = 150;
  sensorDistInt();
  bothWheelsGo(lowSpeed, highSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(rightFrwdMaxSpeed, leftFrwdMaxSpeed, halfSec);
  sensorDistInt();
  bothWheelsGo(lowSpeed, highSpeed, quarterSec);
  sensorDistInt();
  bothWheelsStop(fullSec);


  sensorDistInt();
  bothWheelsGo(lowSpeed, lowSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(rightFrwdMaxSpeed, leftRvrsMaxSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(lowSpeed, lowSpeed, quarterSec);
  sensorDistInt();
  bothWheelsStop(fullSec);

  sensorDistInt();
  bothWheelsGo(highSpeed, lowSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(rightRvrsMaxSpeed, leftRvrsMaxSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(highSpeed, lowSpeed, quarterSec);
  sensorDistInt();
  bothWheelsStop(fullSec);

  sensorDistInt();
  bothWheelsGo(highSpeed, highSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(rightRvrsMaxSpeed, leftFrwdMaxSpeed, quarterSec);
  sensorDistInt();
  bothWheelsGo(highSpeed, highSpeed, quarterSec);
  sensorDistInt();
  bothWheelsStop(fullSec);


}
