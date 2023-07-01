// main.h
#include <arduino.h>
#include "MP3lib.h"
#include <SoftwareSerial.h>
#include "DistSensor.h"
#include "TorsoControl.h"

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

void setup()
{
  // put your setup code here, to run once:
  delay(50);
  Serial.begin(9600);
  Serial.println("Serial initialized");
  pinMode(testLed, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  mp3.begin(9600);
  listenMP3();
  torso.setSpeed(150);
}
void loop()
{
  if (!playing and (distRead()< 90 ))
  {
    TorsoGoto(-100);
    sendMP3Command('p');
    Serial.println("SentPlay");
    delay(1000);
    TorsoGoto(100);
    delay(1000);
  }
  TorsoGoto(0);
  // Serial.println(distRead());
  // sendMP3Command('u');
  listenMP3();
}
void SerialEvent()
{
  while (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    Serial.println("command recieved" + command);
    if (command.length() == 1)
    {
      sendMP3Command(command.charAt(0));
    }
  }
  blinkTestLED();
}
void mp3Event()
{
  listenMP3();
  blinkTestLED();
}