// DistSensor.h
/********distance sensor*******/
#define trigPin 5
#define echoPin 4

extern long duration;
extern int distance;
extern int del;
extern bool trig;

/*********************************************************************/
extern int distances[5];
extern int i;
extern int dist_place;
extern int avdist;

int singleRead();
int distRead();