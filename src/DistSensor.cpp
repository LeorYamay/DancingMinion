// DistSensor.cpp
#include <arduino.h>
#include "DistSensor.h"
#include <NewPing.h>

NewPing sonar(trigPin, echoPin, 200);

int DistByAvg(int samples)
{
  return sonar.convert_cm(sonar.ping_median(samples));
}