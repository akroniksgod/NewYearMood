#include <SoftwareSerial.h>
#include <TimerOne.h>
#include "VoiceRecognitionV3.h"
VR myVR(2, 3);    // 2:RX 3:TX, you can choose your favourite pins.

/*
* Перечень допустимых режимов.
* ON - включаем все лампочки.
* OFF - выключаем все лампочки.
* BLINKING - режим мигания.
* CHANGED_COLOUR - режим включения определённого цвета.
*/
enum Modes {
  ON,
  OFF,
  BLINKING,
  RED,
  BLUE,
  GREEN,
};

uint8_t records[7];
uint8_t buf[64];

/*
* Номер пина для соответствующего цвета.
* Красный - 13;
* Синий - 8;
* Зелёный - 7;
*/
int pinRed = 13;
int pinBlue = 8;
int pinGreen = 7;

/*
* Текущий режим подстветки.
*/
Modes mode = OFF;

/*
* Перечень команд, записанных в плате.
*/
#define changeMode (0)
#define turnRed (1)
#define turnGreen (2)
#define turnBlue (3)

//init functions
void turnOff();
void turnOn();
void turnBlinkingOn();
void turnRedColourOn();
void turnBlueColourOn();
void turnGreenColourOn();
//

/
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
    int i;
    for(i = 0; i < len; i++) {
        if (buf[i]>0x19 && buf[i]<0x7F) {
            Serial.write(buf[i]);
        } else {
            Serial.print("[");
            Serial.print(buf[i], HEX);
            Serial.print("]");
        }
    }
}

/
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
    Serial.println("VR Index\tGroup\tRecordNum\tSignature");
    Serial.print(buf[2], DEC);
    Serial.print("\t\t");
    if (buf[0] == 0xFF) {
        Serial.print("NONE");
    } else if (buf[0]&0x80) {
        Serial.print("UG ");
        Serial.print(buf[0]&(~0x80), DEC);
    } else {
        Serial.print("SG ");
        Serial.print(buf[0], DEC);
    }
    Serial.print("\t");
    Serial.print(buf[1], DEC);
    Serial.print("\t\t");
    if (buf[3] > 0) {
        printSignature(buf+4, buf[3]);
    } else {
        Serial.print("NONE");
    }
    Serial.println("\r\n");
}

void setup()
{
    int speed = 9600;
    myVR.begin(speed);
    Serial.begin(115200);
    Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
    pinMode(pinRed, OUTPUT);
    pinMode(pinBlue, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    if (myVR.clear() == 0) {
        Serial.println("Recognizer cleared.");
    } else {
        Serial.println("Not find VoiceRecognitionModule.");
        Serial.println("Please check connection and restart Arduino.");
        while(1);
    }
    if (myVR.load((uint8_t)changeMode) >= 0) {
        Serial.println("shazam was loaded");
    }
    if (myVR.load((uint8_t)turnRed) >= 0) {
        Serial.println("red mode was loaded");
    }
    if (myVR.load((uint8_t)turnGreen) >= 0) {
        Serial.println("green mode was loaded");
    }
    if (myVR.load((uint8_t)turnBlue) >= 0) {
        Serial.println("blue mode was loaded");
    }
    Timer1.initialize(100000);
}

void timerIsr()
{
    digitalWrite(pinRed, digitalRead(pinRed) ^ 1);
    digitalWrite(pinBlue, digitalRead(pinBlue) ^ 1);
    digitalWrite(pinGreen, digitalRead(pinGreen) ^ 1);
}

void loop()
{
    int timeout = 100;
    int ret = myVR.recognize(buf, timeout);
    
    if (ret > 0) { 
        switch(buf[1])
        {
            case changeMode:
              switch (mode) 
              {
                case ON: turnBlinkingOn(); break;
                case OFF: turnOn(); break;
                case BLINKING: turnOff(); break;
                case RED: 
                case GREEN:
                case BLUE: turnOff(); break;
              }
              break;
            case turnRed: turnRedColourOn(); break;
            case turnBlue: turnBlueColourOn(); break;
            case turnGreen: turnGreenColourOn(); break;
            default:
                Timer1.detachInterrupt();
                Serial.println("Record function undefined");
                break;
        }
        printVR(buf);
    }
}

/*
* Выполняет выключение подсветки.
*/
void turnOff() 
{
  //вместо HIGH должно быть LOW
  mode = OFF;
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinBlue, HIGH);
  digitalWrite(pinGreen, HIGH);
  Timer1.detachInterrupt();
}

/*
* Выполняет включение подсветки.
*/
void turnOn()
{
  //вместо LOW должно быть HIGH
  mode = ON;
  digitalWrite(pinRed, LOW);
  digitalWrite(pinBlue, LOW);
  digitalWrite(pinGreen, LOW);
  Timer1.detachInterrupt();
}

/*
* Выполняет включение режима мигания.
*/
void turnBlinkingOn()
{
  mode = BLINKING;                  
  Timer1.attachInterrupt(timerIsr);
}

/*
* Выполняет включение красного цвета цвета.
*/
void turnRedColourOn()
{
  //вместо LOW должно быть HIGH и наоборот
  mode = RED;
  digitalWrite(pinRed, LOW);
  digitalWrite(pinGreen, HIGH);
  digitalWrite(pinBlue, HIGH);
  Timer1.detachInterrupt();
}

/*
* Выполняет включение синего цвета.
*/
void turnBlueColourOn()
{
  //вместо LOW должно быть HIGH и наоборот
  mode = BLUE;
  digitalWrite(pinBlue, LOW);
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinGreen, HIGH);
  Timer1.detachInterrupt();
}

/*
* Выполняет включение зёленого цвета.
*/
void turnGreenColourOn()
{
  //вместо LOW должно быть HIGH и наоборот
  mode = GREEN;
  digitalWrite(pinGreen, LOW);
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinBlue, HIGH);
  Timer1.detachInterrupt();
}