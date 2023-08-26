//ArmsControl.h
#include <Servo.h>

#define servoR 10
#define servoL 9

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
void ArmsSleep();
void ArmsWake();