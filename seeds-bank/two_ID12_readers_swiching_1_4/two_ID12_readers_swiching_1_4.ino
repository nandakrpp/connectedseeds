/* 
  two_ID12_readers_switching
  
  reads two data from rfid readers on an Arduino UNO
  by switching between two software serail ports at 2
  seconds interval.

  Required library: SoftwareSerial

  Required hardware:
  1 Arduino UNO
  2 Innovations ID12 or ID20
  a few 125kHz RFID tags

  Setup: two_ID12_RFID_readers_1_UNO_bb.png
  modified from: jCoder's Reading RFID tags with Arduino and ID12:
  <http://jcoder.me/blog/2012/06/19/reading-rfid-tags-with-arduino-and-id12/>

  Modified from: 
  two_ID12_readers_switching_1_4
  jCoder's code <http://blog.jcoder.me/files/arduino/RFIDReaderExample.txt>

  Last modified: 2016-08-12
  By Nanda Khaorapapong
  Contact nanda.krpp@gmail.com
  
  License: 
  Copyright (c) 2016 Nanda Khaorapapong and Connected Seeds 
  MIT License <https://opensource.org/licenses/MIT>

*/

#include <SoftwareSerial.h>

SoftwareSerial mySerialOne(2, 3); // RX, TX
SoftwareSerial mySerialTwo(4, 5); // RX, TX
unsigned long lapse = 0;
unsigned long INTERVAL = 2000;
int ledCheck = 13;




void setup()
{
  mySerialOne.begin(9600);
  mySerialTwo.begin(9600);
  Serial.begin(9600);
  pinMode(ledCheck, OUTPUT);
}




void loop ()
{
  for (int i = 1; i < 3; i++)
  {
    getReader(i);
  }
}











