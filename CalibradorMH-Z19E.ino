// =================================================================//
//  UTILITY SKETCH: MH-Z19E - Manual Zero-Point Calibration
//  WARNING: Use with caution. Misuse can de-calibrate the sensor.
// =================================================================//

#include <MHZ19.h>

// I'll use Serial1 for this operation to match my final project's pinout.
HardwareSerial mhzSerial(1); 
MHZ19 myMHZ19;

void setup() {
  Serial.begin(115200);
  
  // I initialize communication on the designated pins.
  mhzSerial.begin(9600, SERIAL_8N1, 2, 15); // ESP32's RX, TX
  myMHZ19.begin(mhzSerial);

  Serial.println("====================- WARNING -====================");
  Serial.println("This sketch will perform a zero-point calibration.");
  Serial.println("ACTION REQUIRED: Ensure the sensor has been in fresh,");
  Serial.println("outdoor air for at least 20 minutes BEFORE running this.");
  Serial.println("===================================================");
  Serial.println("Sending calibration command NOW...");

  // This is the line that sends the calibration command.
  // I will only run this sketch ONCE to perform the calibration.
  myMHZ19.calibrate(); 

  Serial.println("Calibration command sent!");
  Serial.println("The sensor will now use the current atmosphere as its 'zero point' (400 ppm).");
  Serial.println("Please re-upload the main project code now.");
}

void loop() {
  // I will leave the loop empty on purpose, as this sketch
  // should only perform its action once in the setup.
  delay(1000);
}