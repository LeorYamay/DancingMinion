//ArmsControl.h
#include <Servo.h>

#define servoR 9
#define servoL 10

extern Servo servoRight;
extern Servo servoLeft;

extern int rightLocate;
extern int leftLocate;
void RightMove(int right);
void LeftMove(int left);
void RightSet(int right);
void LeftSet(int left);
int ServoRange(int number);
void ArmsHome();
void RandomArms();