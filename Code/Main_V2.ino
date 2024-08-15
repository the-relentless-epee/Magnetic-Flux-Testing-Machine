//==========================libraries
#include <Arduino.h>
#include "BasicStepperDriver.h"

#include <SPI.h>
#include <SD.h>
//==========================pin definitions

# define M1 10//dc driver
# define M2 11//dc driver
# define EN 13
# define STEP 16//stepper
# define DIR 15//stepper
# define ADC 26
# define endStop 1
const int _MISO = 4;  // AKA SPI RX
const int _MOSI = 3;  // AKA SPI TX
const int _CS = 5;
const int _SCK = 2;

//==========================variables

//stepper
#define MS 4
#define MOTOR_STEPS 200
#define RPM 60
#define STEPS_PER_MM 5

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, EN);


//logging
#define samplesCount 10000 //samples taken per measurement cycle
#define sampleTime 5000 //sampling time in milliseconds
#define samplingInterval samplesCount/sampleTime*1000 //interval between samples in us
#define sampleDelay 1000 //delay to let voltage settle before sampling


//movement
#define moveDistance 30 //total testing distance in mm
#define stepDistance 0.1 //distance of each step in mm

#define coilDistance 60 //distance from home to coil in mm
#define initialDistance 0.2 //distance to start from coil in mm

float currentPos = 0;  //actual distance from home, in mm
int cycleCount = moveDistance/stepDistance; 
unsigned long prevMicros = 0;
unsigned long currentMicros = 0;


//==================================functions
//homing
void home(){
  Serial.println("homing");
  while (digitalRead(endStop)==1){
    stepper.move(-0.5*STEPS_PER_MM*MS); //course homing moves 0.5mm at a time
  }
  delay(100);
  stepper.move(5*STEPS_PER_MM*MS); //move back 5mm to reset the switch
  while(digitalRead(endStop)==1){ //fine homing moves one step at a time
    stepper.move(-1);
    delay(10);
  }
  Serial.println("homed");

  stepper.move(coilDistance*STEPS_PER_MM*MS);
  delay(100);
  stepper.move(-initialDistance*STEPS_PER_MM*MS);
  currentPos = coilDistance-initialDistance;
  Serial.println("homed and ready");
}
//DC motor 
void spinMotor(){
  digitalWrite(M1, 1);
  analogWrite(M2, 255); //simplified to go full speed all the time
}
void stopMotor(){
  digitalWrite(M1, 1);
  digitalWrite(M2, 1);
}
//data logging
void logData(int distance){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  for (int i = 0; i<(samplesCount+1); i++){
    currentMicros = micros();
    while (currentMicros-prevMicros<samplingInterval){
      int data = analogRead(ADC);
      if (dataFile) {
        dataFile.print(distance);
        dataFile.print(", ");
        dataFile.println(data);
        //dataFile.close(); s
        Serial.print(distance);
        Serial.print(", ");
        Serial.println(data);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
        break;
      }
      prevMicros = currentMicros;
    }
  }
}

//====================================================
void setup(){
  //Serial
  Serial.begin(115200);

  //SD
  Serial.println("initializing SD card");
  bool sdInitialized = false;
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
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  //pin definition
  pinMode(endStop, INPUT_PULLDOWN);
  pinMode(ADC, INPUT);

  //stepper
  stepper.begin(RPM, MS);
  stepper.setEnableActiveState(LOW);
  stepper.enable(); 

  //variables
  currentPos = 0;
  delay(1000);
}

void loop(){
  home();
  File dataFile = SD.open("dataLog.txt", FILE_WRITE|O_TRUNC);
  dataFile.println("start");
  spinMotor();
  delay(1500);  
  for (int i = initialDistance; i<=moveDistance; i += stepDistance){
    delay(sampleDelay);
    logData(i);
    delay(100);
    stepper.move(-(stepDistance*STEPS_PER_MM*MS));
  }
  delay(100);
  stopMotor();
  dataFile.println("finished");
  while(true){
    stepper.disable();
  }
}





















