//ArmsControl.cpp
#include "ArmsControl.h"
#include <Servo.h>

Servo servoRight;  
Servo servoLeft; 

int rightLocate = 0;
void RightMove(int right)
{
  rightLocate += right;
  servoRight.write(rightLocate);

}
void RightSet(int right)
{
    rightLocate = right;
    servoRight.write(rightLocate);
}
int leftLocate = 180;
void LeftMove(int left)
{
  leftLocate += 180-left;
  servoLeft.write(leftLocate);
}
void LeftSet(int left)
{
    leftLocate = 180-left;
    servoLeft.write(leftLocate);
}
