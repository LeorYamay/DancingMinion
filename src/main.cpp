// main.h
#include <arduino.h>
#include "MP3lib.h"
#include <SoftwareSerial.h>
#include "DistSensor.h"
#include "TorsoControl.h"
#include "ArmsControl.h"


bool go = false;
int dist = -10;
unsigned long elapsedTime = -10;
unsigned long startTime = -10;
void setup()
{
  // put your setup code here, to run once:
  delay(50);
  Serial.begin(9600);
  Serial.println("Serial initialized");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // servoRight.attach(servoR);
  // servoLeft.attach(servoL);
  mp3Setup();
  ListenMP3();
  torso.setSpeed(200);
  torso.release();
  startTime = millis();
  elapsedTime = 5000;
}
int track = 1;
bool done = false;
void loop()
{
  dist = DistByAvg(7);
  go = (3 <= dist) and (dist <= 30);
  Serial.println(String(go) + "---" + String(elapsedTime) + "----" + String(playing));
  if (playing or (elapsedTime < 5000))
  {
    RandomTorso();
    RandomArms();
    RandomTorso();

    // SendMP3Command('w');
    if (!playing and (random(0, 10) < 1))
    {
      Serial.println("Random Combo!!");
      PlayRandom();
    }
  }
  else
  {
    if (!done)
    {
      TorsoHome();
      ArmsHome();
      delay(500);
      done = true;
      ArmsSleep();
    }
    else
    {
      ArmsSleep();
    }
    
  }
  if (!playing and go and (elapsedTime > 5000))
  {
    Serial.println("Started");
    startTime = millis();
    PlayRandom();
    done = false;
  }
  if (!ack and playing)
  {
    Serial.println("Too long without finishing");
    startTime = millis() - 5000;
    SendMP3Command('W');
    // StopSound();
  }
  ListenMP3();
  torso.release();
  elapsedTime = millis() - startTime;
  if (elapsedTime < 0)
  {
    startTime = millis() - 5000;
    elapsedTime = 5001;
  }
}
void SerialEvent()
{
  while (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    Serial.println("command recieved" + command);
    if (command.length() == 1)
    {
      SendMP3Command(command.charAt(0));
    }
  }
  // blinkTestLED();
}
// void mp3Event()
// {
//   ListenMP3();
//   // blinkTestLED();
// }