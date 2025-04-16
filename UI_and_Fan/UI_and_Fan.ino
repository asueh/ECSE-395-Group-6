/*
  This code is for Spring 2025 ECSE 395 Group 6's project. 
  It is meant to be a humidifier that works with an external water source for easy cleaning.
  The current code is to run the fan (Motor B) for 5 seconds whenever the actual humidity is below the target humidity.
  It then waits for 30 seconds before turning the fan on for another 5, repeating until the actual humidity is
  at or above the target humidity.
  The board used for this project is an Seeed Studio XIAO ESP32S3 as it had enough analog out pins for the project
  While the board given in class, the Adafruit Feather V2, does not have enough.
*/

#include <DHT.h> //for DHT11 Sensor
#include <LiquidCrystal_I2C.h> //for LCD screen

//lcd is wired to SCL and SDA pins
const int DHTPIN = A10;  // Define the pin used to connect the sensor (A0)
#define DHTTYPE DHT11  // Define the sensor type
const int PotPIN = A9; // Defines the potentiometer pin (A1)

const int motorB_1A = A1; //Define the pin for pin B-1A on motor controller board
const int motorB_2A = A0; //Define the pin for pin B-2A on motor controller board
const int motorA_1A = A3; //Define the pin for pin A-1A on motor controller board
const int motorA_1B = A2; //Define the pin for pin A-1B on motor controller board

LiquidCrystal_I2C lcd(0x27, 16, 2);  // initialize the Liquid Crystal Display object with the I2C address 0x27, 16 columns and 2 rows

DHT dht(DHTPIN, DHTTYPE);  // Create a DHT object

void setup() {
  // Initialize the serial communication
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //sets all the motor pins as output
  pinMode(motorB_1A, OUTPUT); 
  pinMode(motorB_2A, OUTPUT);  
  pinMode(motorA_1A, OUTPUT);  
  pinMode(motorA_1B, OUTPUT); 

  Serial.begin(9600);

  dht.begin();  // Initialize the DHT sensor

  int target_base = 40; //smallest value for target humidity

  //Sets up the lcd screen immediately after turning on
  lcd.setCursor(0,0);
  lcd.print("Current: ");
  lcd.print(read_Humidity());
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Target: ");
  lcd.print(read_Pot() + target_base);
  lcd.print("%");


}


int last_time_motor = millis();
int last_time_hum = millis();
int last_Pot_Val = 0;
int target_base = 40; //smallest value for target humidity
int actual_humidity = read_Humidity();
int target_humidity = read_Pot() + target_base;

void loop() {
  // DHT-11 CODE

  int current_time = millis();
  int time_diff_hum = current_time - last_time_hum;
  int current_Pot_Val = read_Pot();
  // checks if at least 2 seconds has elapsed since last humidity reading
  if (time_diff_hum >= 2000) {
    int actual_humidity = read_Humidity();
    lcd.setCursor(0,0); //Set cursor to character 0 on line 0

    //Prints the current humidity read by the DHT11
    lcd.print("Current: ");
    lcd.print(actual_humidity);
    lcd.print("%");
  }


  // POTENTIOMETER CODE

  //displays the current potentiometer value and doesn't update serial unless value has changed
  if (current_Pot_Val != last_Pot_Val) {
    // updates last and current potentiometer values
    last_Pot_Val = current_Pot_Val;
    current_Pot_Val = read_Pot();

    int target_humidity = current_Pot_Val + target_base; // user set target humidity
    //Serial.println(target_humidity);
    lcd.setCursor(0,1); //Set cursor to character 0 on line 1
    lcd.print("Target: ");
    lcd.print(target_humidity);
    lcd.print("%");
  }

  // MOTOR CODE
  int current_time_motor = millis();
  int time_diff_motor = current_time_motor - last_time_motor;
  if (actual_humidity < target_humidity) {
    if (time_diff_motor < 5000) {
      run_Fan();
    } else {
      fan_Off();
    }
    if (time_diff_motor > 35000) {
      last_time_motor = millis();
    }
  }
}

// reads and returns the humidity from the DHT sensor
int read_Humidity() {
  int humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return humidity;
  }
  //Serial.print(F("Humidity: "));
  //Serial.println(humidity);
  last_time_hum = millis();
  return humidity;
}

//gets a value from 0 to 20 from the potentiometer
int read_Pot() {
  int Pot_Val = analogRead(PotPIN);
  int pot_final_val = (Pot_Val * 20) / 4095;
  return pot_final_val;
}

void run_Fan() {
  analogWrite(motorB_1A, 255);
  analogWrite(motorB_2A, 0);
}

void fan_Off() {
  analogWrite(motorB_1A, 0);
  analogWrite(motorB_2A, 0);
}