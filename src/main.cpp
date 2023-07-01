// main.h
#include <arduino.h>
#include "MP3lib.h"
#include <SoftwareSerial.h>

void setup()
{
  // put your setup code here, to run once:
  delay(50);
  Serial.begin(9600);
  Serial.println("Serial initialized");
  mp3.begin(9600);
}
void loop()
{
  // if (!playing)
  // {
  //   sendMP3Command('p');
  //   Serial.println("SentPlay");
  // }
}
void Serialevemt_mp3()
{
  listenMP3();
}
void Serialevent()
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
