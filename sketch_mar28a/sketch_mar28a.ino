#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTTYPE DHT11  // Define the sensor type

const int DHTPIN = 26;  // Define the pin used to connect the sensor (A0)
const int PotPIN = 25; // Defines the potentiometer pin (A1)

DHT dht(DHTPIN, DHTTYPE);  // Create a DHT object

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int target_base = 40; // smallest value for target humidity
int last_time = millis();
int last_Pot_Val = 0;

int count = 0;

void setup()
{
  // Initialize the serial communication
  Serial.begin(115200);
  Serial.println(F("DHT11 test!"));

  dht.begin();  // Initialize the DHT sensor

  lcd.init();// initialize the lcd 
  lcd.backlight(); // Turns on the LCD backlight.
  lcd.print("Hello, world!");   // Print a message to the LCD.
  delay(3000); 
}

void loop()
{
  lcd.clear(); 
  lcd.setCursor(0, 0); // Sets the cursor position to the first row and first column (0, 0).

  int current_time = millis();
  int time_diff = current_time - last_time;
  int current_Pot_Val = read_Pot();
  // checks if at least 2 seconds has elapsed since last humidity reading
  if (time_diff >= 2000) 
  {
    float actual_humidity = read_Humidity();
  }
  //displays the current potentiometer value and doesn't update serial unless value has changed
  if (current_Pot_Val != last_Pot_Val) 
  {
    // updates last and current potentiometer values
    last_Pot_Val = current_Pot_Val;
    current_Pot_Val = read_Pot();

    int target_humidity = current_Pot_Val + target_base; // user set target humidity
    Serial.println(target_humidity);
  }

}

int read_Humidity() 
{
  int humidity = dht.readHumidity();
  if (isnan(humidity)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return humidity;
  }
  lcd.print("HUMIDITY: ");
  lcd.print(humidity); // Prints the current value of the humidity variable.
  last_time = millis();
  return humidity;
}

//gets a value from 0 to 20 from the potentiometer
int read_Pot() 
{
  int Pot_Val = analogRead(PotPIN);
  int pot_final_val = (Pot_Val * 20) / 4095;
  return pot_final_val;
}
