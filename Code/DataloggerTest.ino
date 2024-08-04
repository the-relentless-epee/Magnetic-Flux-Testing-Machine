/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows (Raspberry Pi Pico):
   ** MISO - pin 4
   ** MOSI - pin 7
   ** CS   - pin 5
   ** SCK  - pin 6

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

// This are GP pins for SPI0 on the Raspberry Pi Pico board, and connect
// to different *board* level pinouts.  Check the PCB while wiring.
// Only certain pins can be used by the SPI hardware, so if you change
// these be sure they are legal or the program will crash.
// See: https://datasheets.raspberrypi.com/picow/PicoW-A4-Pinout.pdf
const int _MISO = 4;  // AKA SPI RX
const int _MOSI = 3;  // AKA SPI TX
const int _CS = 5;
const int _SCK = 2;

byte sensorPin = 26;

#include <SPI.h>
#include <SD.h>

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  while (!Serial) {
    delay(1);  // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("\nInitializing SD card...");

  bool sdInitialized = false;
  // Ensure the SPI pinout the SD card is connected to is configured properly
  // Select the correct SPI based on _MISO pin for the RP2040


  SPI.setRX(_MISO);
  SPI.setTX(_MOSI);
  SPI.setSCK(_SCK);
  sdInitialized = SD.begin(_CS);


  if (!sdInitialized) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
}

//-----------------------------------------------------------------------------------------------------------------
void loop() {

  int data = analogRead(sensorPin);
    


  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    Serial.println(data);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}









