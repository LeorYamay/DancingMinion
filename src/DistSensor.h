// DistSensor.h
/********distance sensor*******/
#define trigPin A2
#define echoPin A1
#include <NewPing.h>

extern NewPing sonar;

int DistByAvg(int samples);
