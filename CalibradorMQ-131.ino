// =================================================================//
//  SENSOR TEST: MQ-131 - Ozone (Raw Analog Reading)
// =================================================================//

// I'm defining the analog input pin I've connected the sensor's AOUT to.
// GPIO35 is a good choice as it's a reliable, input-only analog pin.
const int o3SensorPin = 35; 

void setup() {
  // I start serial communication to see the output.
  Serial.begin(9600);
  Serial.println("MQ-131 Ozone sensor test initiated...");
  Serial.println("The sensor requires a warm-up period for readings to stabilize.");
}

void loop() {
  // In the loop, I'm reading the raw analog value from the pin.
  // The ESP32's ADC has a 12-bit resolution, so the value will be between 0 and 4095.
  int o3Value = analogRead(o3SensorPin);

  // I print the raw value. A higher value typically indicates lower ozone concentration for MQ sensors.
  Serial.print("Ozone sensor raw value: ");
  Serial.println(o3Value);
  
  // I'll pause for 2 seconds before the next reading.
  delay(2000);
}