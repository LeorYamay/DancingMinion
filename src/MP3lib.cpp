// MP3lib.cpp
#include <arduino.h>
#include <SoftwareSerial.h>
#include "MP3lib.h"

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
bool playing = false;

#pragma region Serialmp3Do
void playthis(int track)
{
    sendCommand(CMD_PLAY_FOLDER_FILE, track);
    playing = true;
}

String mess = "";
void playthat(int track, int plus)
{
    stopsound();
    playthis(track + plus);
    mess = "playing: ";
    mess += String(track + plus, HEX);
    Serial.println(mess);
}

void pausesound()
{
    if (playing)
        sendCommand(CMD_PAUSE, 0);
    else
        sendCommand(CMD_PLAY, 0);
    playing = !playing;
}
void stopsound()
{
    if (playing)
        sendCommand(CMD_STOP_PLAY, 0);
    playing = false;
}
void  play_all(int track, int num)
{
    int i = 0;
    playthat(track, i);
    while (i < num)
    {
        if (!playing)
        {
            i++;
            playthat(track, i);
        }
    }
}
#pragma endregion

#pragma region soundCardExtras
/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
void sendCommand(int8_t command, int16_t dat)
{
    delay(20);
    Send_buf[0] = 0x7e;               //
    Send_buf[1] = 0xff;               //
    Send_buf[2] = 0x06;               // Len
    Send_buf[3] = command;            //
    Send_buf[4] = 0x01;               // 0x00 NO, 0x01 feedback
    Send_buf[5] = (int8_t)(dat >> 8); // datah
    Send_buf[6] = (int8_t)(dat);      // datal
    Send_buf[7] = 0xef;               //
    for (uint8_t i = 0; i < 8; i++)
    {
        mp3.write(Send_buf[i]);
    }
}

/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */

String sbyte2hex(uint8_t b)
{
    String shex;

    // Serial.print("0x");
    shex = "0X";

    // if (b < 16) Serial.print("0");
    if (b < 16)
        shex += "0";
    // Serial.print(b, HEX);
    shex += String(b, HEX);
    // Serial.print(" ");
    shex += " ";
    return shex;
}

/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

String sanswer(void)
{
    uint8_t i = 0;
    String mp3answer = "";

    // Get only 10 Bytes
    while (mp3.available() && (i < 10))
    {
        uint8_t b = mp3.read();
        ansbuf[i] = b;
        i++;

        // Serial.print(sbyte2hex(b));
        mp3answer += sbyte2hex(b);
    }

    // Serial.println();

    // if the answer format is correct.
    if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
    {
        // return true;
        return mp3answer;
    }

    // return false;
    return "???: " + mp3answer;
}

/********************************************************************************/
/*Function sendMP3Command: seek for a 'c' command and send it to MP3  */
/*Parameter: c. Code for the MP3 Command, 'h' for help.                                                                                                         */
/*Return:  void                                                                */

void sendMP3Command(char c)
{
    switch (c)
    {
    case '?':
    case 'h':
        Serial.println("HELP  ");
        Serial.println(" p > Play / Pause ");
        Serial.println(" n > Next");
        Serial.println(" b > Previous");
        Serial.println(" u > Volume UP");
        Serial.println(" d > Volume DOWN");
        break;

    case 'p':
        if (!playing)
        {
            Serial.println("Play");
            // sendCommand(CMD_PLAY_W_VOL, 0x1E);//play the first song with volume 15 class
            sendCommand(CMD_PLAY, 0);
            playing = true;
        }
        else
        {
            Serial.println("Pause");
            sendCommand(CMD_PAUSE, 0);
            playing = false;
        }
        break;

    case 'n':
        Serial.println("Next");
        sendCommand(CMD_NEXT_SONG, 0);
        sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
        break;

    case 'b':
        Serial.println("Previous");
        sendCommand(CMD_PREV_SONG, 0);
        sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
        break;

    case 'u':
        Serial.println("Volume Up");
        sendCommand(CMD_VOLUME_UP, 0);
        break;

    case 'd':
        Serial.println("Volume Down");
        sendCommand(CMD_VOLUME_DOWN, 0);
        break;
    }
}

/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

String decodeMP3Answer()
{
    String decodedMP3Answer = "";

    decodedMP3Answer += sanswer();

    //  if (ansbuf[3] == 0x4C) // currently planying
    //  {
    //    decodedMP3Answer+=" -> Playing: "+String(ansbuf[6],DEC);
    //  }

    switch (ansbuf[3])
    {
    case 0x3A:
        decodedMP3Answer += " -> Memory card inserted.";
        break;
    
    case 0x3B:
        decodedMP3Answer += " -> Memory card ejected.";
        break;

    case 0x3D:
        decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
        playing = false;
        break;

    case 0x4C:
        decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
        playing = true;
        break;

    case 0x41:
        decodedMP3Answer += " -> Data recived correctly. ";
        break;
    }

    return decodedMP3Answer;
}
void listenMP3()
{
    while (mp3.available())
    {
        Serial.println(decodeMP3Answer());
    }
}
#pragma endregion