/*
  This code reads the temperature and humidity values from a DHT11 sensor 
  connected to pin 25 and prints them to the serial monitor. 
  It also calculates and prints the heat index value in both Celsius and Fahrenheit.
  
  Board: ESP32 Development Board
  Component: Temperature and humidity module(DHT11)
  Library: https://github.com/adafruit/DHT-sensor-library  (DHT sensor library by Adafruit)
*/

#include <DHT.h>

const int DHTPIN = 26;  // Define the pin used to connect the sensor (A0)
#define DHTTYPE DHT11  // Define the sensor type
const int PotPIN = 25; // Defines the potentiometer pin (A1)


DHT dht(DHTPIN, DHTTYPE);  // Create a DHT object

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));

  dht.begin();  // Initialize the DHT sensor
}


int target_base = 40; // smallest value for target humidity
int last_time = millis();
int last_Pot_Val = 0;


void loop() {
  int current_time = millis();
  int time_diff = current_time - last_time;
  int current_Pot_Val = read_Pot();
  // checks if at least 2 seconds has elapsed since last humidity reading
  if (time_diff >= 2000) {
  float actual_humidity = read_Humidity();
  }
  //displays the current potentiometer value and doesn't update serial unless value has changed
  if (current_Pot_Val != last_Pot_Val) {
    // updates last and current potentiometer values
    last_Pot_Val = current_Pot_Val;
    current_Pot_Val = read_Pot();

    int target_humidity = current_Pot_Val + target_base; // user set target humidity
    Serial.println(target_humidity);
  }
  

}

int read_Humidity() {
  int humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return humidity;
  }
  Serial.print(F("Humidity: "));
  Serial.println(humidity);
  last_time = millis();
  return humidity;
}

//gets a value from 0 to 20 from the potentiometer
int read_Pot() {
  int Pot_Val = analogRead(PotPIN);
  int pot_final_val = (Pot_Val * 20) / 4095;
  return pot_final_val;
}