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

//=========================variables
int MS = 2;
#define MOTOR_STEPS 200
#define RPM 60
int currentPos;
int coilDistance = 300*MS; //distance from coil to homing position

#define STEPS_PER_MM 5
#define samplesCount 100000 //samples per cycle
#define duration 5000//duration of each cycle (ms)
#define moveDistance 40 //length of movement in mm
int cycleCount = STEPS_PER_MM*moveDistance*MS; //number of steps
#define stepSize 5*MS*5 //size of each step in testing routine
#define stepDelay 1000

//==========================initializations
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, EN);


//=========================functions
//homing
void home() {
  while (digitalRead(endStop)==1){
    stepper.move(-2);
    currentPos -= 2;
    //Serial.println(currentPos);
    //delay(1);
  }
  delay(100);
  stepper.move(MS*10);
  while(digitalRead(endStop)==1){
    stepper.move(-1);
    currentPos -= 1;
    //Serial.println(currentPos);
    delay(100);
  }
  currentPos = 0;
  Serial.println("homed");
}

//dc motor
void spinMotor(int speed){
  digitalWrite(M1, 1);
  analogWrite(M2, speed);
}

//braking
void stopMotor(){
  digitalWrite(M1, 1);
  digitalWrite(M2, 1);
}

//data
void logData(int time, int samples, int distance){
  int interval = time*1000/samples;
  int prevmicros = 0;
  int currentmicros = 0;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (samples/time<499999){
    for (int i = 0; i<samples; i++){
      //File dataFile = SD.open("datalog.txt", FILE_WRITE);
      while(currentmicros-prevmicros < interval){
        currentmicros = micros();
      }
      prevmicros=currentmicros;
      int data = analogRead(ADC);
      if (dataFile) {
        dataFile.print(distance);
        dataFile.print("\t");
        dataFile.println(data);
        //dataFile.close(); 1
        Serial.print(distance);
        Serial.print(", ");
        Serial.println(data);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
        break;
      }
    }
  }
  else{
    Serial.println("requested sample rate too high");
  }
}


//=======================================================
void setup(){
  Serial.begin(115200);
  /*while (!Serial) {
    //delay(1);  // wait for serial port to connect. Needed for native USB port only
  }*/
  
  //section SD
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

  //section pin
  pinMode(endStop, INPUT_PULLDOWN);
  pinMode(ADC, INPUT);

  //section stepper
  stepper.begin(RPM, MS);
  stepper.setEnableActiveState(LOW);
  stepper.enable();
  
  currentPos = 0;
}

void loop(){
  delay(5000);
  Serial.println("homing");
  home();
  stepper.move(coilDistance);
  currentPos = coilDistance;
  delay(1000);
  stepper.move(-2);
  delay(1000);
  spinMotor(255);
  delay(1000);
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("//starting//");
  logData(duration, samplesCount, 200);
  dataFile.close();
  for(int i = 0; i<=(cycleCount/stepSize/10); i++){
    stepper.move(-stepSize);
    int distance = (i*stepSize)*100+200; //microns
    delay(stepDelay);
    logData(duration, samplesCount, distance);
  }
  dataFile.println("//finished//");
  dataFile.close();
  stopMotor();
  while(true){
    stepper.disable();
  }
}
































