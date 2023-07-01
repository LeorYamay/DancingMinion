// torsoControl.cpp
#include <AFMotor.h>
#include "TorsoControl.h"
#include <Arduino.h>

// Change this as per your motor's specification
const int stepsPerRevolution = 32;

// connect motor to port #2 (M3 and M4)
AF_Stepper torso(stepsPerRevolution, 2);

int torso_location = 0;

void TorsoStep(int steps)
{
  torso_location += steps;
  torso.step(abs(steps), dir(steps), MICROSTEP);
}
void TorsoGoto(int location)
{
  int delta = location - torso_location;
  TorsoStep(delta);
}

int dir(int steps)
{
   if (steps >= 0)
  {
    return FORWARD;
  }
  else
  {
    return BACKWARD;
  }
}