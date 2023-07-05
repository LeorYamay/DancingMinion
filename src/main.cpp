// main.h
#include <arduino.h>
#include "MP3lib.h"
#include <SoftwareSerial.h>
#include "DistSensor.h"
#include "TorsoControl.h"
#include "ArmsControl.h"

#pragma region testLed
#define testLed 13
void blinkTestLED()
{
  // Blink the test LED three times
  for (int iterate = 0; iterate < 3; iterate++)
  {
    digitalWrite(testLed, HIGH); // Turn on the test LED
    delay(500);                  // Wait for 500 milliseconds
    digitalWrite(testLed, LOW);  // Turn off the test LED
    delay(500);                  // Wait for 500 milliseconds
  }
}
#pragma endregion
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
  pinMode(testLed, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoRight.attach(servoR);
  servoLeft.attach(servoL);
  mp3.begin(9600);
  ListenMP3();
  torso.setSpeed(200);
  torso.release();
  startTime = millis();
  elapsedTime = 5000;
}
int track = 1;
void loop()
{
  dist = DistByAvg(7);
  go = (3 <= dist) and (dist <= 30);
  Serial.println(String(go) + "---" + String(elapsedTime) + "----" + String(playing));
  if (playing or (elapsedTime < 5000))
  {
    RandomTorso();
    RandomArms();
  }
  else
  {
    TorsoHome();
    ArmsHome();
  }
  if (!playing and go and (elapsedTime > 5000))
  {
    Serial.println("Started");
    startTime = millis();
    PlayRandom();
    
  }
  elapsedTime = millis() - startTime;
  if ((elapsedTime > 10000) and playing)
  {
    Serial.println("Too long without finishing");
    StopSound();
  }
  ListenMP3();
  torso.release();
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