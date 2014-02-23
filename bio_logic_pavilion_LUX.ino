#include <Servo.h>

Servo servoL;
Servo servoR;

int luxL = 0;
int luxR = 1;

int valL = 0;
int valR = 0;

int leftMinVal = 1023;
int rightMinVal = 1023;

int leftMaxVal = 0;
int rightMaxVal = 0;

int topL = 180;
int topR = 180;

int bottomL = 30;
int bottomR = 30;

//smoothing variables
const int numReadings = 5;
int index = 0;

int readingsL[numReadings];
int readingsR[numReadings];

int totalL = 0;
int totalR = 0;
int averageL = 0;
int averageR = 0;


void setup()
{
  //attach servos to pins
  servoL.attach(9);
  servoR.attach(10);

  //set up serial communication
  Serial.begin(9600);

  //run calibration function
  calibrate();
}

void loop()
{
  //read the photoresistors
  valL = analogRead(luxL);
  valR = analogRead(luxR);

  ///////// MAP VALUES ///////////////////////
  //////////////////////////////
  //contrain readings to avoid issues with  mapping
  constrain(valL, leftMinVal, leftMaxVal);
  constrain(valR, rightMinVal, rightMaxVal);

  //map values to servo locations
  valL = map(valL, leftMinVal, leftMaxVal+1, bottomL, topL);
  valR = map(valR, rightMinVal-1, rightMaxVal+1, bottomR, topR);

  Serial.print("readL: ");
  Serial.print(valL);
  Serial.print(" || readR: ");
  Serial.print(valR);



  //servoR is mounted upside down, so we inverse the value
  valR = topR-valR + 30;
  valL = topL-valL + 30;
  
  //smooth it out
  totalL = totalL - readingsL[index];
  totalR = totalR - readingsR[index];
  
  readingsL[index] = valL;
  readingsR[index] = valR;
  
  totalL = totalL + readingsL[index];
  totalR = totalR + readingsR[index];
  
  index = index + 1;
  
  if(index >= numReadings){
    index = 0;
  } 
  
  averageL = totalL/numReadings;
  averageR = totalR/numReadings;
  
  valL = averageL;
  valR = averageR;

  //write to servos
  servoL.write(valL);
  servoR.write(valR);

  //print the final Values
  Serial.print(" ||||| left Location: ");
  Serial.print(valL);
  Serial.print(" || right location: ");
  Serial.println(valR);

  delay(20);
}


///////////////calibration/////////////////////////////////
void calibrate(void)
{
  Serial.println("calibrating...ramp light up and down.....");
  while (millis() < 10000) {
    valL = analogRead(luxL);
    valR = analogRead(luxR);

    // record the maximum sensor value
    if (valL > leftMaxVal) {
      leftMaxVal = valL;
    }

    if (valR > rightMaxVal) {
      rightMaxVal = valR;
    }

    // record the minimum sensor value
    if (valL < leftMinVal) {
      leftMinVal = valL;
    }

    if (valR < rightMinVal) {
      rightMinVal = valR;
    }
  }

  Serial.print("leftMax: ");
  Serial.print(leftMaxVal);
  Serial.print(" rightMax: ");
  Serial.print(rightMaxVal);
  Serial.print(" || leftMin: ");
  Serial.print(leftMinVal);
  Serial.print(" rightMin: ");
  Serial.println(rightMinVal);
}





