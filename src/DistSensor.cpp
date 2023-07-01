//DistSensor.cpp
#include <arduino.h>
#include "DistSensor.h"

long duration;
int distance;
int del = 400;
bool trig = false;

/*********************************************************************/
int distances[5] = {0, 0, 0, 0, 0};
int i;
int dist_place = 1;
int avdist = 0;

int singleRead()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  return distance;
}

int distRead()
{
  avdist = 0;
  for (i = 0; i <= 4; i = i + 1) {
    distances[i] = singleRead();
    avdist += distances[i];
  }
  //distances[0] = distance;
  //avdist += distances[0];
  avdist = avdist / 5;
  // Prints the distance on the Serial Monitor
  // Serial.print("distance: ");
  // Serial.println(avdist);
  return avdist;
}
