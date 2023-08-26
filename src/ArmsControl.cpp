// ArmsControl.cpp
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
  Serial.println("Right " + String(rightLocate));
}
void RightSet(int right)
{
  rightLocate = right;
  rightLocate = ServoRange(rightLocate);
  ArmsWake();
  servoRight.write(rightLocate);
  Serial.println("Right " + String(rightLocate));
}
int leftLocate = 180;
void LeftMove(int left)
{
  leftLocate -= left;
  leftLocate = ServoRange(leftLocate);
  servoLeft.write(leftLocate);
  Serial.println("Left " + String(leftLocate));
}
void LeftSet(int left)
{
  leftLocate = 180 - left;
  leftLocate = ServoRange(leftLocate);
  ArmsWake();
  servoLeft.write(leftLocate);
  Serial.println("Left " + String(leftLocate));
}
const int stepMax = 30;
void RandomArms()
{
  int leftStep = random(-stepMax, stepMax);
  int rightStep = random(-10, 10 );
 ArmsWake();
  LeftMove(leftStep);
  delay(50);
  RightMove(rightStep);
  delay(50);
  // Serial.println("Random Right:" + String(rightLocate) +" Random Left:" +String(leftLocate));
}
int ServoRange(int number)
{
  if (number > 200)
  {
    return 180;
  }
  if (number < -20)
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
void ArmsSleep()
{
  servoRight.detach();
  servoLeft.detach();
}
void ArmsWake()
{
 if (!servoLeft.attached())
  {
    servoLeft.attach(servoL);
  }
  if (!servoRight.attached())
  {
    servoRight.attach(servoR);
  }
}