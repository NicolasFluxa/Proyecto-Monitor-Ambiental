// =================================================================//
//  SENSOR TEST: GY-BMP280 - Barometric Pressure & Temperature
// =================================================================//

// I'm including the required libraries for I2C communication and the BMP280 sensor.
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// I'll create an object for the BMP280 sensor. By default, the library
// will use the standard I2C pins (SDA: 21, SCL: 22 on most ESP32 boards).
Adafruit_BMP280 bmp; 

void setup() {
  // I start serial communication for debugging.
  Serial.begin(9600);
  Serial.println("BMP280 sensor test initiated...");

  // In setup, I'll try to initialize the sensor. The begin() function returns
  // true on success and false on failure. 0x76 is the default I2C address for this sensor.
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    // If the sensor is not found, I'll halt the program here.
    while (1); 
  }
}

void loop() {
  // I read the temperature from the sensor.
  Serial.print("Temperature: ");
  Serial.print(bmp.readTemperature());
  Serial.print(" *C  |  ");

  // I'm reading the pressure, which the library provides in Pascals (Pa).
  // I will divide it by 100.0F to convert it to hPa (Hectopascals), a more common unit.
  Serial.print("Pressure: ");
  Serial.print(bmp.readPressure() / 100.0F);
  Serial.println(" hPa");

  // I'll wait 2 seconds before the next measurement.
  delay(2000);
}