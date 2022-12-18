#include <SoftwareSerial.h>
#include <TimerOne.h>
#include "VoiceRecognitionV3.h"

VR myVR(2, 3);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t records[7];
uint8_t buf[64];
int led = 13;
int ledG = 9;

#define onRecord    (0)
#define offRecord   (1)
#define blinkRecord (2)
#define RedRecord (3)

/**
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

/**
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
    myVR.begin(9600);
    Serial.begin(115200);
    Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
    pinMode(led, OUTPUT);
    pinMode(ledG, OUTPUT);
    if (myVR.clear() == 0) {
        Serial.println("Recognizer cleared.");
    } else {
        Serial.println("Not find VoiceRecognitionModule.");
        Serial.println("Please check connection and restart Arduino.");
        while(1);
    }
    if (myVR.load((uint8_t)onRecord) >= 0) {
        Serial.println("onRecord loaded");
    }
    if (myVR.load((uint8_t)offRecord) >= 0) {
        Serial.println("offRecord loaded");
    }
    if (myVR.load((uint8_t)blinkRecord) >= 0) {
        Serial.println("blinkRecord loaded");
    }
    if (myVR.load((uint8_t)RedRecord) >= 0) {
        Serial.println("RedRecord loaded");
    }
    Timer1.initialize(100000);
}

void timerIsr1()
{
    digitalWrite(13, digitalRead(13) ^ 1);
}

void timerIsr2()
{
    digitalWrite(8, digitalRead(8) ^ 1);
}
void loop()
{
    int ret;
    ret = myVR.recognize(buf, 100);    
    if (ret > 0) { 
        switch(buf[1]){
			case 0:
                digitalWrite(13, LOW);
                digitalWrite(8, LOW);
                digitalWrite(7, LOW);
                Timer1.detachInterrupt();
                break;
            case 1:
                digitalWrite(13, HIGH);
                digitalWrite(8, HIGH);
                digitalWrite(7, HIGH);
                Timer1.detachInterrupt();
                break;
            case 2:
                digitalWrite(13, HIGH);
                Timer1.detachInterrupt();
                break;
            case 3:
                digitalWrite(7, HIGH);
                Timer1.detachInterrupt();
                break;
            case 4:
                digitalWrite(8, HIGH);
                Timer1.detachInterrupt();
                break;
            default:
                Timer1.detachInterrupt();
                Serial.println("Record function undefined");
                break;
        }
        printVR(buf);
    }
}