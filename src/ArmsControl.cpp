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
void RandomArms()
{
  int leftStep = random(-20,20);
  int rightStep = random(-20,20);
  LeftMove(leftStep);
  RightMove(rightStep);
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