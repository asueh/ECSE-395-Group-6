/*
  This code defines and initializes a motor, ultrasonic sensor and joystick.

  Board: ESP32 Development Board
  Component: Motor and L9110 motor control board
*/

// Define the motor pins
const int motorB_1A = 26;
const int motorB_2A = 25;

const int motorA_1A = 39;
const int motorA_2A = 34;

void setup() 
{
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  pinMode(motorB_1A, OUTPUT);  // set motor pin 1 as output
  pinMode(motorB_2A, OUTPUT);  // set motor pin 2 as output

  pinMode(motorA_1A, OUTPUT);  // set pump pin 1 as output
  pinMode(motorA_2A, OUTPUT);  // set pump pin 2 as output

  digitalWrite(motorA_1A, LOW); 
  digitalWrite(motorA_2A, HIGH);

  delay(5000);// wait for 5 seconds

  digitalWrite(motorA_1A, LOW);  // turn off the pump
  digitalWrite(motorA_2A, LOW);
}

void loop() 
{
  analogWrite(motorB_1A, 250);  // set motor speed
  analogWrite(motorB_2A, 0);
  delay(50);
}

