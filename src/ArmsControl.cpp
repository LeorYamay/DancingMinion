//ArmsControl.cpp
#include "ArmsControl.h"
#include <Servo.h>
#include <Arduino.h>

Servo servoRight;  
Servo servoLeft; 

int rightLocate = 0;
void RightMove(int right)
{
  rightLocate += right;
  rightLocate = ServoRange(rightLocate);
  servoRight.write(rightLocate);

}
void RightSet(int right)
{
    rightLocate = right;
    rightLocate = ServoRange(rightLocate);
    servoRight.write(rightLocate);
}
int leftLocate = 180;
void LeftMove(int left)
{
  leftLocate += 180-left;
  leftLocate = ServoRange(leftLocate);
  servoLeft.write(leftLocate);
}
void LeftSet(int left)
{
    leftLocate = 180-left;
    leftLocate = ServoRange(leftLocate);
    servoLeft.write(leftLocate);
}
const int stepMax =30;
void RandomArms()
{
  int leftStep = random(-stepMax,stepMax);
  int rightStep = random(-stepMax,stepMax);
  LeftMove(leftStep);
  RightMove(rightStep);
  Serial.println("Random Right:" + String(rightLocate) +" Random Left:" +String(leftLocate));
}
int ServoRange(int number)
{
  if (number>180)
  {
    return 180;
  }
  if (number<0)
  {
    return 0;
  }
  return number;
}
void ArmsHome()
{
  RightSet(90);
  LeftSet(90);
}