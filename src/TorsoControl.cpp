// torsoControl.cpp
#include <AFMotor.h>
#include "TorsoControl.h"
#include <Arduino.h>

// Change this as per your motor's specification
const int stepsPerRevolution = 32;

// connect motor to port #2 (M3 and M4)
AF_Stepper torso(stepsPerRevolution, 2);

int torsoLocation = 0;
int stepMax = 90;
void TorsoStep(int steps)
{
  torsoLocation += steps;
  if (abs(torsoLocation) > 90)
  {
    torsoLocation -= steps;
    TorsoGoto(sgn(torsoLocation)*90);
  }
  else
  {
    torso.step(abs(steps), dir(steps), MICROSTEP);
  }
}
void TorsoGoto(int location)
{
  int delta = location - torsoLocation;
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
int sgn(int number)
{
  if (number == 0)
  {
    return 0;
  }
  else{
    return (number/abs(number));
  }
}

void RandomTorso()
{
  int randomStep = random(-stepMax,stepMax);
  TorsoStep(randomStep);
}

void TorsoHome()
{
  if(torsoLocation !=0)
  {
    TorsoGoto(0);
  }
}