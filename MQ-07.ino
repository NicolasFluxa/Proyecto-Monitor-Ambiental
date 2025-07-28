// =================================================================//
//  SENSOR TEST: MQ-7 - Carbon Monoxide (Raw Analog Reading)
// =================================================================//

// I'm defining the analog input pin I've connected the sensor's AOUT to.
// GPIO34 is a good choice as it's a reliable, input-only analog pin.
const int mq7Pin = 34;

void setup() {
  // I start serial communication to see the output.
  Serial.begin(9600);
  Serial.println("MQ-7 Carbon Monoxide sensor test initiated...");
  Serial.println("The sensor requires a warm-up period (several minutes) for readings to stabilize.");
}

void loop() {
  // In the loop, I'm reading the raw analog value from the pin.
  // The ESP32's ADC has a 12-bit resolution, so the value will be between 0 and 4095.
  int coValue = analogRead(mq7Pin);

  // I print the raw value.
  Serial.print("CO sensor raw value: ");
  Serial.println(coValue);

  // I'll pause for 2 seconds before the next reading.
  delay(2000);
}
