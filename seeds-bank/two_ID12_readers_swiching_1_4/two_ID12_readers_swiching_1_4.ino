---
title: " two_ID12_readers_switching"
author: "nandakrpp"
date: "18 August 2016"
output: html_document
---
## two_ID12_readers_switching

### Description:
  Reading 125kHz RFID tags via two RFID readers on an Arduino by switching between two software serail ports (each is connected to an RFID reader) at 2 seconds interval.
  
### Required hardware:
* An Arduino UNO
* Two Innovations ID12 or ID20
* At lesat one 125kHz RFID tag
  
### Setup: 
  
  ![two_ID12_RFID_readers_1_UNO_bb.png](http://nandadoes.com/download_seeds/docu_media/two_ID12_RFID_readers_1_UNO_bb.png) _Reference:_ [jCoder's Reading RFID tags with Arduino and ID12](
  <http://jcoder.me/blog/2012/06/19/reading-rfid-tags-with-arduino-and-id12/)
  
  
### Required Library:
[SerialSoftware](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/SoftwareSerial/src)

### Modified from: 
  * two_ID12_readers_switching_1_4
  * [jCoder's code](http://blog.jcoder.me/files/arduino/RFIDReaderExample.txt)
  
  
### Last modified: 
Date: 2016-08-12  
By: Nanda Khaorapapong  
Contact: nanda.krpp@gmail.com
  
### License: 
Copyright (c) 2016 Nanda Khaorapapong and Connected Seeds    
[MIT License](https://opensource.org/licenses/MIT)







```objc
/*
  two_ID12_readers_switching_1_4
  
  Reading 125kHz RFID tags via two RFID readers on an Arduino by switching 
  between two software serail ports (each is connected to an RFID reader) 
  at 2 seconds interval.

  Library: SoftwareSerial

  Hardware:
  1 Arduino UNO
  2 Innovations ID12 or ID20
  a few 125kHz RFID tags

  Setup: two_ID12_RFID_readers_1_UNO_bb.png
  modified from: jCoder's Reading RFID tags with Arduino and ID12:
  <http://jcoder.me/blog/2012/06/19/reading-rfid-tags-with-arduino-and-id12/>

  Modified from: jCoder's code
  <http://blog.jcoder.me/files/arduino/RFIDReaderExample.txt>

  Last modified: 2016-08-11
  By Nanda Khaorapapong
  Contact nanda.krpp@gmail.com

  License: Copyright (c) 2016 Nanda Khaorapapong and Connected Seeds
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




void getReader(int r)
{

  Serial.print("reader ");
  Serial.println(r);

  lapse = millis();
  while ( (millis() - lapse <= INTERVAL) )
  {
    byte i = 0;
    byte val = 0;
    byte code[6];
    byte checksum = 0;
    byte bytesread = 0;
    byte tempbyte = 0;



    if (r == 1)
    {
      mySerialOne.listen();
      if (mySerialOne.available() > 0)
      {
        info();
        if ((val = mySerialOne.read()) == 2)
        { // check for header
          info();
          bytesread = 0;
          while (bytesread < 12)
          { // read 10 digit code + 2 digit checksum
            if ( mySerialOne.available() > 0)
            {
              val = mySerialOne.read();
              info();
              if ((val == 0x0D) || (val == 0x0A) || (val == 0x03) || (val == 0x02))
              { // if header or stop bytes before the 10 digit reading
                break;                                    // stop reading
              }

              // Do Ascii/Hex conversion:
              if ((val >= '0') && (val <= '9'))
              {
                val = val - '0';
              }
              else if ((val >= 'A') && (val <= 'F'))
              {
                val = 10 + val - 'A';
              }

              // Every two hex-digits, add byte to code:
              if (bytesread & 1 == 1)
              {
                // make some space for this hex-digit by
                // shifting the previous hex-digit with 4 bits to the left:
                code[bytesread >> 1] = (val | (tempbyte << 4));

                if (bytesread >> 1 != 5)
                { // If we're at the checksum byte,
                  checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
                };
              }
              else
              {
                tempbyte = val;                           // Store the first hex digit first...
              };

              bytesread++;                                // ready to read next digit
            }
          }

        }

      }
    }




    else
    {
      mySerialTwo.listen();
      if (mySerialTwo.available() > 0)
      {
        info();
        if ((val = mySerialTwo.read()) == 2)
        { // check for header
          info();
          bytesread = 0;
          while (bytesread < 12)
          { // read 10 digit code + 2 digit checksum
            if ( mySerialTwo.available() > 0)
            {
              val = mySerialTwo.read();
              info();
              if ((val == 0x0D) || (val == 0x0A) || (val == 0x03) || (val == 0x02))
              { // if header or stop bytes before the 10 digit reading
                break;                                    // stop reading
              }
              // Do Ascii/Hex conversion:
              if ((val >= '0') && (val <= '9'))
              {
                val = val - '0';
              }
              else if ((val >= 'A') && (val <= 'F'))
              {
                val = 10 + val - 'A';
              }

              // Every two hex-digits, add byte to code:
              if (bytesread & 1 == 1)
              {
                // make some space for this hex-digit by
                // shifting the previous hex-digit with 4 bits to the left:
                code[bytesread >> 1] = (val | (tempbyte << 4));

                if (bytesread >> 1 != 5)
                { // If we're at the checksum byte,
                  checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
                };
              }
              else
              {
                tempbyte = val;                           // Store the first hex digit first...
              };

              bytesread++;                                // ready to read next digit
            }
          }

        }

      }
    }



    // Output to Serial:

    if (bytesread == 12)
    { // if 12 digit read is complete
      Serial.print("5-byte code: ");
      for (i = 0; i < 5; i++)
      {
        if (code[i] < 16) Serial.print("0");
        Serial.print(code[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      Serial.print("Checksum: ");
      Serial.print(code[5], HEX);
      Serial.println(code[5] == checksum ? " -- passed." : " -- error.");
      Serial.println();
    }

    bytesread = 0;



  }
  lapse = millis();
  Serial.println("===");

}


void info()
{
  digitalWrite(ledCheck, HIGH);
  delay(30);
  digitalWrite(ledCheck, LOW);
  delay(30);
}

```
