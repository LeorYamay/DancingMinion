// main.h
#include <arduino.h>
#include "MP3lib.h"
#include <SoftwareSerial.h>
#include "DistSensor.h"

#pragma region testLed
#define testLed 13
void blinkTestLED()
{
  // Blink the test LED three times
  for (int iterate = 0; iterate < 3; iterate++)
  {
    digitalWrite(testLed, HIGH);  // Turn on the test LED
    delay(500);              // Wait for 500 milliseconds
    digitalWrite(testLed, LOW);   // Turn off the test LED
    delay(500);              // Wait for 500 milliseconds
  }
}
#pragma endregion
void listenSerial()
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
}

void setup()
{
  // put your setup code here, to run once:
  delay(50);
  Serial.begin(9600);
  Serial.println("Serial initialized");
  pinMode(testLed, OUTPUT);
  mp3.begin(9600);
  listenMP3();
}
void loop()
{
  // if (!playing)
  // {
  //   sendMP3Command('p');
  //   Serial.println("SentPlay");
  // }
  Serial.println(singleRead());
  sendMP3Command('u');
  listenMP3();
  listenSerial();
}
void SerialEvent()
{
  blinkTestLED();
}
void mp3Event()
{
  blinkTestLED();
}