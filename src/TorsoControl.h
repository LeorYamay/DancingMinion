//torsoControl.h
#include <AFMotor.h>
extern int torso_location;
extern AF_Stepper torso;

void torso_step(int steps);
void TorsoGoto(int location);
int dir(int steps);