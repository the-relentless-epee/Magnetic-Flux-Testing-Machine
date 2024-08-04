/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 60

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 2

// All the wires needed for full functionality
#define DIR 15
#define STEP 16
#define EN 13
#define switchPin 14
//Uncomment line to use enable/disable functionality
//#define SLEEP 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, EN);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

//step/mm = 5

int currentPos;
int coilDistance = 300*MICROSTEPS;

void home() {
  while (digitalRead(switchPin)==1){
    stepper.move(-2);
    currentPos -= 2;
    Serial.println(currentPos);
    delay(1);
  }
  delay(100);
  stepper.move(MICROSTEPS*10);
  while(digitalRead(switchPin)==1){
    stepper.move(-1);
    currentPos -= 1;
    Serial.println(currentPos);
    delay(100);
  }
  currentPos = 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT_PULLDOWN);
  stepper.begin(RPM, MICROSTEPS);
  stepper.setEnableActiveState(LOW);
  
  stepper.enable();
  stepper.move(MICROSTEPS*2);
  currentPos = 0;
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  
}

void loop(){
  home();
  stepper.move(coilDistance);
  currentPos = coilDistance;
  Serial.println(currentPos);
  while(true){
    stepper.disable();
  }
}


