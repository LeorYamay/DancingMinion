//torsoControl.h
#include <AFMotor.h>
extern int torsoLocation;
extern AF_Stepper torso;

void torso_step(int steps);
void TorsoGoto(int location);
int dir(int steps);
int sgn(int number);
void RandomTorso();
void TorsoHome();