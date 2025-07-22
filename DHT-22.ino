// =================================================================//
//  SENSOR TEST: DHT22 - Temperature & Humidity
// =================================================================//

// First, I include the necessary libraries from Adafruit.
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Here, I define the GPIO pin my sensor is connected to and its specific type.
#define DHTPIN 4      // The GPIO pin I'm using for the DATA line.
#define DHTTYPE DHT22 // I specify that my sensor model is the DHT22.

// I create a DHT object that the rest of my code will use to interact with the sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // I'll start serial communication to see the output on my computer's Serial Monitor.
  Serial.begin(9600); 
  Serial.println("DHT22 sensor test initiated!");

  // I initialize the sensor itself.
  dht.begin();
}

void loop() {
  // The DHT22 datasheet recommends waiting at least 2 seconds between readings.
  delay(2000);

  // In the main loop, I'll read the humidity and temperature from the sensor.
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read in Celsius by default.

  // It's good practice to check if the readings are valid. Sometimes they can fail.
  // isnan() checks if the value is "Not a Number".
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return; // I'll exit the function early if there's an error.
  }

  // If the reading was successful, I print the data to the Serial Monitor.
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  |  ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}