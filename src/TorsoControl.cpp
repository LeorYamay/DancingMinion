//torsoControl.cpp
#include "TorsoControl.h"
#include <Stepper.h>


Stepper torso = Stepper(32, stepper1,
                        stepper2,
                        stepper3,
                        stepper4);
int torso_location = 0;

void TorsoStep(int steps)
{
  torso_location += steps;
  torso.step(steps);
}
void TorsoGoto(int location)
{
  torso.step(location-torso_location);
  torso_location = location;
}