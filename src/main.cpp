#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// MP3 definitions
#include <SoftwareSerial.h>
#define ARDUINO_RX 2
#define ARDUINO_TX 3

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

static int8_t Send_buf[8] = {0};
static uint8_t ansbuf[10] = {0};

static int8_t pre_vol, volume = 0x0f;
String mp3Answer;
boolean playing = false;

/************ Command byte **************************/
#define CMD_NEXT_SONG 0X01
#define CMD_PREV_SONG 0X02
#define CMD_PLAY_W_INDEX 0X03
#define CMD_VOLUME_UP 0X04
#define CMD_VOLUME_DOWN 0X05
#define CMD_SET_VOLUME 0X06
#define CMD_SNG_CYCL_PLAY 0X08
#define CMD_SEL_DEV 0X09
#define CMD_SLEEP_MODE 0X0A
#define CMD_WAKE_UP 0X0B
#define CMD_RESET 0X0C
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F
#define CMD_STOP_PLAY 0X16
#define CMD_FOLDER_CYCLE 0X17
#define CMD_SHUFFLE_PLAY 0x18
#define CMD_SET_SNGL_CYCL 0X19
#define CMD_SET_DAC 0X1A
#define DAC_ON 0X00
#define DAC_OFF 0X01
#define CMD_PLAY_W_VOL 0X22
#define CMD_PLAYING_N 0x4C

/************ Options **************************/
#define DEV_TF 0X02
#define SINGLE_CYCLE_ON 0X00
#define SINGLE_CYCLE_OFF 0X01

/********* Arm servo stuff *********/
#define servoR 7
#define servoL 12

Servo servoRight;
Servo servoLeft;

/**************** Torso stepper stuff ***********/
#define stepper1 8
#define stepper2 10
#define stepper3 9
#define stepper4 11

Stepper torso = Stepper(32, stepper1, stepper2, stepper3, stepper4);
int torso_location = 0;

void torso_step(int steps) {
  torso_location += steps;
  torso.step(steps);
  Serial.print("Torso location: ");
  Serial.println(torso_location);
}

int right_location = 0;
void right_move(int right) {
  right_location += right;
  Serial.print("Right location: ");
  Serial.println(right_location);
  servoRight.write(right_location);
}

int left_location = 180;
void left_move(int left) {
  left_location -= left;
  Serial.print("Left location: ");
  Serial.println(left_location);
  servoLeft.write(left_location);
}

/******** Distance sensor *******/
#define trigPin 5
#define echoPin 4

long duration;
int distance;
int del = 400;
bool trig = false;

/*********************************************************************/
int distances[5] = {0, 0, 0, 0, 0};
int i;
int dist_place = 1;

int singleRead() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void averageRead() {
  distances[dist_place] = singleRead();
  dist_place++;
  if (dist_place > 4)
    dist_place = 0;
  distance = 0;
  for (i = 0; i <= 4; i++)
    distance += distances[i];
  distance = distance / 5;
  Serial.print(distance);
  Serial.println(" average cm");
}
/***********************************************************************/

void sendCommand(int8_t command, uint16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e;    // starting byte
  Send_buf[1] = 0xff;    // version
  Send_buf[2] = 0x06;    // the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;    // 0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8); // datah
  Send_buf[6] = (int8_t)(dat);      // datal
  Send_buf[7] = 0xef;               // ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]) ;
  }
}

void mp3_set_serial() {
  mp3.begin(9600);
}

void setup() {
  Serial.begin(9600);
  mp3_set_serial();
  servoRight.attach(servoR);
  servoLeft.attach(servoL);
  torso.setSpeed(50);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(1000);
  Serial.println("Initializing...");
  delay(1000);
  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(500);
  sendCommand(CMD_SET_VOLUME, volume);
  delay(500);
}

void loop() {
  averageRead();

  if (distance < del && trig == false) {
    delay(200);
    sendCommand(CMD_PAUSE, 0);
    delay(200);
    sendCommand(CMD_STOP_PLAY, 0);
    delay(200);
    sendCommand(CMD_PLAY_W_INDEX, 1);
    delay(200);
    trig = true;
  }

  if (distance > del && trig == true) {
    delay(200);
    sendCommand(CMD_PAUSE, 0);
    delay(200);
    sendCommand(CMD_STOP_PLAY, 0);
    delay(200);
    sendCommand(CMD_PLAY_W_INDEX, 2);
    delay(200);
    trig = false;
  }

  delay(100);
}