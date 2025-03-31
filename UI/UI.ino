/*
  This code is for Spring 2025 ECSE 395 Group 6's project. 
  It is meant to be a humidifier that works with an external water source for easy cleaning.
  The current code is for the UI, measuring the humidity of the room,
  and getting a user set target humidity for the room and displays that on an LCD screen.
  The board used for this project is an Adafruit Feather ESP32 V2.
*/

#include <DHT.h> //for DHT11 Sensor
#include <LiquidCrystal_I2C.h> //for LCD screen

//lcd is wired to SCL and SDA pins
const int DHTPIN = 26;  // Define the pin used to connect the sensor (A0)
#define DHTTYPE DHT11  // Define the sensor type
const int PotPIN = 25; // Defines the potentiometer pin (A1)

LiquidCrystal_I2C lcd(0x27, 16, 2);  // initialize the Liquid Crystal Display object with the I2C address 0x27, 16 columns and 2 rows

DHT dht(DHTPIN, DHTTYPE);  // Create a DHT object

void setup() {
  // Initialize the serial communication
  lcd.init();
  lcd.clear();
  lcd.backlight();


  Serial.begin(9600);

  dht.begin();  // Initialize the DHT sensor

  int target_base = 40;

  lcd.setCursor(0,1);
  lcd.print("Target: ");
  lcd.print(read_Pot() + target_base);
  lcd.print("%");
}


 // smallest value for target humidity
int last_time = millis();
int last_Pot_Val = 0;
int target_base = 40;

void loop() {
  int current_time = millis();
  int time_diff = current_time - last_time;
  int current_Pot_Val = read_Pot();
  // checks if at least 2 seconds has elapsed since last humidity reading
  if (time_diff >= 2000) {
  int actual_humidity = read_Humidity();
  lcd.setCursor(0,0); //Set cursor to character 0 on line 0

  //TEST THIS
  lcd.print("Current: ");
  lcd.print(actual_humidity);
  lcd.print("%");
  }
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
  

}

int read_Humidity() {
  int humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return humidity;
  }
  //Serial.print(F("Humidity: "));
  //Serial.println(humidity);
  last_time = millis();
  return humidity;
}

//gets a value from 0 to 20 from the potentiometer
int read_Pot() {
  int Pot_Val = analogRead(PotPIN);
  int pot_final_val = (Pot_Val * 20) / 4095;
  return pot_final_val;
}