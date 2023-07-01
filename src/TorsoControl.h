//torsoControl.h
#include <Stepper.h>

extern Stepper torso;
#define stepper1 11
#define stepper2 9
#define stepper3 10
#define stepper4 8

extern int torso_location;

void torso_step(int steps);
void TorsoGoto(int location);