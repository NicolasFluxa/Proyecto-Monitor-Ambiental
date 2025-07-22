// =================================================================//
//  SENSOR TEST: MH-Z19E - Carbon Dioxide (CO₂)
// =================================================================//

// First, I include the dedicated library for the MH-Z19 sensor.
#include <MHZ19.h>

// I'll define the pins I'm using for the UART communication.
#define RX_PIN 16 // This is the ESP32's RX pin, connected to the sensor's TX.
#define TX_PIN 17 // This is the ESP32's TX pin, connected to the sensor's RX.

// I create an object for my MH-Z19 sensor.
MHZ19 myMHZ19;
// I'll use the second hardware serial port for communication.
HardwareSerial mhzSerial(2);

void setup() {
  // I start serial communication to see the output.
  Serial.begin(9600);
  
  // I initialize the serial port for the sensor.
  mhzSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  // I associate the library object with the serial port I've configured.
  myMHZ19.begin(mhzSerial);

  // I'm enabling the auto-calibration feature. This feature assumes the sensor
  // will be exposed to fresh air (~400 ppm) at least once in its operational cycle.
  myMHZ19.autoCalibration(true);

  Serial.println("MH-Z19E CO2 sensor test initiated...");
  Serial.println("The sensor needs 1-3 minutes to warm up for stable readings.");
}

void loop() {
  // In the loop, I use the library's getCO2() function to easily get the reading in ppm.
  int co2 = myMHZ19.getCO2();

  // I print the result to the Serial Monitor.
  Serial.print("CO2 Concentration: ");
  Serial.print(co2);
  Serial.println(" ppm");

  // I wait for 5 seconds before the next reading, as the sensor updates slowly.
  delay(5000);
}