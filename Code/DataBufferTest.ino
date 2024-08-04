/*
Arduino Uno
Circuit:
  analog sensor connected to pin A0

This is a test of using an array as a temporary data buffer. The ideal behavior would be to read the ADC at full sample rate, record the data to
a temporary string/array, output the string to the serial monitor once it reaches a certain size (not so big that it overflows), then resets the 
array to store the new variable.
*/
# include "string.h"

int sensorPin = A0;
const int bufferSize = 100;
int data1[bufferSize];
int data2[bufferSize];
int val;


void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  for (int i=0;i<bufferSize;i++){
    data2[i] = "/0";
  }
  Serial.println("initialized");
  delay(500);

}

void loop() {
  for (int i=0;i < bufferSize; i++){
    data1[i] = analogRead(sensorPin);
    Serial.println(data2[i]);
    delay(10);
  }
  for (int i=0; i<bufferSize; i++){
    data2[i] = analogRead(sensorPin);
    Serial.println(data1[i]);
    delay(10);
  }
}
