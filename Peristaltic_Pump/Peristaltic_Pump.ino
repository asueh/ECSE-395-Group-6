
//      ******************************************************************
//      *                                                                *
//      *         Simple example for controlling a stepper motor         *
//      *                                                                *
//      *            S. Reifel & Co.                6/24/2018            *
//      *                                                                *
//      ******************************************************************


// This is the simplest example of how to run a stepper motor.  
//
// Documentation for this library can be found at:
//    https://github.com/Stan-Reifel/SpeedyStepper
//
//
// This library requires that your stepper motor be connected to the Arduino 
// using drive electronics that has a "Step and Direction" interface.  
// Examples of these are:
//
//    Pololu's DRV8825 Stepper Motor Driver Carrier:
//        https://www.pololu.com/product/2133
//
//    Pololu's A4988 Stepper Motor Driver Carrier:
//        https://www.pololu.com/product/2980
//
//    Sparkfun's Big Easy Driver:
//        https://www.sparkfun.com/products/12859
//
//    GeckoDrive G203V industrial controller:
//        https://www.geckodrive.com/g203v.html
//
// For all driver boards, it is VERY important that you set the motor 
// current before running the example.  This is typically done by adjusting
// a potentiometer on the board.  Read the driver board's documentation to 
// learn how.

// ***********************************************************************


#include <SpeedyStepper.h>


//
// pin assignments
//
const int EN_PIN = D5;
const int MOTOR_STEP_PIN = D8;
const int MOTOR_DIRECTION_PIN = D9;
const int WaterLevelPIN = A1;


//
// create the stepper motor object
//
SpeedyStepper stepper;



void setup() 
{
  //
  // setup the LED pin and enable print statements
  //
  pinMode(EN_PIN, OUTPUT);  
  Serial.begin(9600);


  //
  // connect and configure the stepper motor to its IO pins
  //
  stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
}



void loop() 
{
  int water_level = analogRead(WaterLevelPIN);
  Serial.println(water_level);
  Serial.println(water_level < 1300);
  if (water_level <= 1400) {
  stepper.setSpeedInStepsPerSecond((1800*3));
  stepper.setAccelerationInStepsPerSecondPerSecond(1800);
  stepper.moveRelativeInSteps(-10000);
  } else {
    stepper.setSpeedInStepsPerSecond(0);
    stepper.setAccelerationInStepsPerSecondPerSecond(0);
    stepper.moveRelativeInSteps(0);
  }
}
