// MP3lib.cpp
#include <arduino.h>
#include <SoftwareSerial.h>
#include "MP3lib.h"
#include <MD_YX5300.h>

SoftwareSerial mp3stream(ARDUINO_RX, ARDUINO_TX);
MD_YX5300 mp3(mp3stream);

bool playing = false;
bool ack = false;
int numTracks = -1;

#pragma region Serialmp3Do
#define PRINT(s, v)         \
    {                       \
        Serial.print(F(s)); \
        Serial.print(v);    \
    }
#define PRINTX(s, v)          \
    {                         \
        Serial.print(F(s));   \
        Serial.print(v, HEX); \
    }
#define PRINTS(s)           \
    {                       \
        Serial.print(F(s)); \
    }
void cbResponse(const MD_YX5300::cbData *status)
// Callback function used to process device unsolicited messages
// or responses to data requests
{
    // PRINTS("\n");
    switch (status->code)
    {
    case MD_YX5300::STS_FILE_END: // track has ended
        PRINTS("STS_FILE_END");
        playing = false;
        // selectNextSong();
        break;

    case MD_YX5300::STS_TF_INSERT: // card has been inserted
        PRINTS("STS_TF_INSERT");
        // S.initializing = initData(true);
        break;

    case MD_YX5300::STS_TF_REMOVE: // card has been removed
        PRINTS("STS_TF_REMOVE");
        playing = false;
        // S.playMode = M_EJECTED;
        // S.playStatus = S_STOPPED;
        // S.needUpdate = true;
        break;

    case MD_YX5300::STS_PLAYING: // current track index
        PRINTS("STS_PLAYING");
        playing = true;
        // S.curTrack = status->data;
        // S.needUpdate = true;
        break;

    case MD_YX5300::STS_FLDR_FILES: // number of files in the folder
        PRINTS("STS_FLDR_FILES");
        numTracks = status->data;
        Serial.println(String(numTracks));
        // S.numTracks = status->data;
        // S.needUpdate = true;
        break;

    // unhandled cases - used for debug only
    case MD_YX5300::STS_VOLUME:
        PRINTS("STS_VOLUME");
        break;
    case MD_YX5300::STS_TOT_FILES:
        PRINTS("STS_TOT_FILES");
        break;
    case MD_YX5300::STS_ERR_FILE:
        PRINTS("STS_ERR_FILE");
        break;
    case MD_YX5300::STS_ACK_OK:
        PRINTS("STS_ACK_OK");
        ack = true;
        break;
    case MD_YX5300::STS_INIT:
        PRINTS("STS_INIT");
        playing = false;
        ack = false;
        break;
    case MD_YX5300::STS_STATUS:
        PRINTS("STS_STATUS");
        break;
    case MD_YX5300::STS_EQUALIZER:
        PRINTS("STS_EQUALIZER");
        break;
    case MD_YX5300::STS_TOT_FLDR:
        PRINTS("STS_TOT_FLDR");
        break;
    default:
        PRINTX("STS_??? 0x", status->code);
        break;
    }

    PRINTX(", 0x", status->data);
    PRINTS("\n");
    //   S.waiting = false;
}

void mp3Setup()
{
    mp3stream.begin(MD_YX5300::SERIAL_BPS);
    mp3.begin();
    mp3.setSynchronous(true);
    mp3.setCallback(cbResponse);
    delay(300);
    while (numTracks < 0)
    {
        Serial.println("-----------");
        Serial.println("query");
        mp3.queryFolderFiles(1);
        delay(1000);
        Serial.println("numtracks = " + String(numTracks));
    }
}
String mess = "";

void PlayThat(int track, int folder)
{
    // int trackValue = (folder << 8) | track;
    ack = false;
    while (!ack)
    {
        mp3.playSpecific(folder, track);
        Serial.println("Requested folder:" + String(folder) + " file:" + String(track));
        mp3.queryFile();
        mp3.check();
        delay(300);
    }
}

void PauseSound()
{
    if (playing)
        mp3.playPause();
    else
        mp3.playStart();
    playing = !playing;
}
void StopSound()
{
    if (playing)
        mp3.playStop();
    playing = false;
}
void PlayAll(int track, int num)
{
    int i = 0;
    PlayThat(track, i);
    while (i < num)
    {
        if (!playing)
        {
            i++;
            PlayThat(track, i);
        }
    }
}
void PlayRandom()
{
    int randomTrack = random(1, numTracks + 1);
    Serial.println("playing random " + String(randomTrack));
    PlayThat(randomTrack, 1);
}
#pragma endregion

#pragma region soundCardExtras
/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */
// static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
// static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
// void sendCommand(int8_t command, int16_t dat)
// {
//     delay(20);
//     Send_buf[0] = 0x7e;               //
//     Send_buf[1] = 0xff;               //
//     Send_buf[2] = 0x06;               // Len
//     Send_buf[3] = command;            //
//     Send_buf[4] = 0x01;               // 0x00 NO, 0x01 feedback
//     Send_buf[5] = (int8_t)(dat >> 8); // datah
//     Send_buf[6] = (int8_t)(dat);      // datal
//     Send_buf[7] = 0xef;               //
//     for (uint8_t i = 0; i < 8; i++)
//     {
//         mp3.write(Send_buf[i]);
//     }
// }

/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */

// String sbyte2hex(uint8_t b)
// {
//     String shex;

//     // Serial.print("0x");
//     shex = "0X";

//     // if (b < 16) Serial.print("0");
//     if (b < 16)
//         shex += "0";
//     // Serial.print(b, HEX);
//     shex += String(b, HEX);
//     // Serial.print(" ");
//     shex += " ";
//     return shex;
// }

/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

// String sanswer(void)
// {
//     uint8_t i = 0;
//     String mp3answer = "";

//     // Get only 10 Bytes
//     while (mp3.available() && (i < 10))
//     {
//         uint8_t b = mp3.read();
//         ansbuf[i] = b;
//         i++;

//         // Serial.print(sbyte2hex(b));
//         mp3answer += sbyte2hex(b);
//     }

//     // Serial.println();

//     // if the answer format is correct.
//     if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
//     {
//         // return true;
//         return mp3answer;
//     }

//     // return false;
//     return "???: " + mp3answer;
// }

/********************************************************************************/
/*Function sendMP3Command: seek for a 'c' command and send it to MP3  */
/*Parameter: c. Code for the MP3 Command, 'h' for help.                                                                                                         */
/*Return:  void                                                                */

void SendMP3Command(char c)
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
            mp3.playStart();
            mp3.queryFile();
            // playing = true;
        }
        else
        {
            Serial.println("Pause");
            mp3.playPause();
            playing = false;
        }
        break;

    case 'n':
        Serial.println("Next");
        mp3.playNext();
        mp3.queryFile();
        break;

    case 'b':
        Serial.println("Previous");
        mp3.playPrev();
        mp3.queryFile();
        break;

    case 'u':
        Serial.println("Volume Up");
        mp3.volumeInc();
        mp3.queryVolume();
        break;

    case 'd':
        Serial.println("Volume Down");
        mp3.volumeDec();
        mp3.queryVolume();
        break;
    case 'r':
        Serial.println("Reset");
        mp3.reset();
        break;
    case 'W':
        Serial.println("Wake up");
        mp3.wakeUp();
        break;
    case 'S':
        Serial.println("Sleep");
        mp3.sleep();
        break;
    }
}

/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

// String decodeMP3Answer()
// {
//     String decodedMP3Answer = "";

//     decodedMP3Answer += sanswer();

//     //  if (ansbuf[3] == 0x4C) // currently planying
//     //  {
//     //    decodedMP3Answer+=" -> Playing: "+String(ansbuf[6],DEC);
//     //  }

//     switch (ansbuf[3])
//     {
//     case 0x3A:
//         decodedMP3Answer += " -> Memory card inserted.";
//         break;

//     case 0x3B:
//         decodedMP3Answer += " -> Memory card ejected.";
//         playing = false;
//         break;

//     case 0x3D:
//         decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
//         playing = false;
//         break;

//     case 0x4C:
//         decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
//         playing = true;
//         break;

//     case 0x42:
//         decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
//         playing = true;
//         break;

//     case 0x41:
//         decodedMP3Answer += " -> Data recived correctly. ";
//         ack = true;
//         break;
//     }

//     return decodedMP3Answer;
// }
void ListenMP3()
{
    mp3.check();
}
#pragma endregion