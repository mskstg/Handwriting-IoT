#include "pitches.h"
#include <Servo.h>

#define red_on 11
#define red_off 10
#define green_on 21
#define green_off 20
#define blue_on 31
#define blue_off 30
#define fan_on 41
#define fan_off 40
#define radio_on 51
#define radio_off 50
#define door_open 61
#define door_close 60


int notes[] = {
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_E4, NOTE_G4, NOTE_F4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, 0,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_A4, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4, NOTE_C5, 0};
  
float durations[] = {
  4, 8, 4, 4, 8, 2, 2, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 8, 8, 2, 2,
  4, 8, 4, 4, 8, 2, 2, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 8, 8, 1.33, 4,
  4, 8, 4, 4, 8, 2, 2, 12, 12, 12,
  8, 8, 8, 8, 8, 8, 2, 2, 5.33, 16,
  8, 4, 4, 8, 5.33, 16, 8, 4, 8, 8,
  8, 2, 4, 4, 1, 1};

const int RedLED = 10;
const int GreenLED = 11;
const int BlueLED = 12;
const int Fan = 5;
const int Door = 9;
const int Radio = 7;

String temp;
int pc_command;

Servo door;
int angle = 100;   //  door flag(closed)

void setup() {
  pinMode(RedLED,OUTPUT);
  pinMode(GreenLED,OUTPUT);
  pinMode(BlueLED,OUTPUT);
  pinMode(Fan,OUTPUT);
  pinMode(6,OUTPUT);   //  fan sub
  door.attach(Door);
  door.write(90);   //  closed
  Serial.begin(9600);
  }

void loop() {
  while(1){
    if(GetCommand()) {
      if(pc_command == radio_on) break;
      CommandCheck();
    }
    delay(100);
  }
  while(1){
    for (int i = 0; i < 76; i++) {

      if(GetCommand()) {
        if(pc_command == radio_off) break;
        CommandCheck();
      }

      float note_duration = 1000/durations[i];
      tone(7, notes[i],note_duration);

      float pause_between = note_duration * 1.30;
      delay(pause_between);
      noTone(7);
    }
    if(pc_command == radio_off) break;
  }
}

int GetCommand(){
  temp = "";
  while(Serial.available()){
    char inputchar = (char)Serial.read();
    temp += inputchar;
  }
  pc_command = temp.toInt();
  return (temp !="");
}

void CommandCheck(){
  switch (pc_command) {
    case red_on : digitalWrite(RedLED,HIGH); break;
    case red_off : digitalWrite(RedLED,LOW); break;
    case green_on : digitalWrite(GreenLED,HIGH); break;
    case green_off : digitalWrite(GreenLED,LOW); break;
    case blue_on : digitalWrite(BlueLED,HIGH); break;
    case blue_off : digitalWrite(BlueLED,LOW); break;
    case fan_on : analogWrite(Fan,120); break;   //  80 ~ 255
    case fan_off : analogWrite(Fan,0); break;
    case door_open : {
      if(angle == 100){
        for (angle = 90; angle >=0; angle = angle - 10) {
          door.write(angle);
          delay(50);
        }
      }
      break;
    }
    case door_close : {
      if(angle == -10){
        for (angle = 0; angle <=90; angle= angle + 10) {
          door.write(angle);
          delay(50);
        }
      }
      break;
    }
  }
}
