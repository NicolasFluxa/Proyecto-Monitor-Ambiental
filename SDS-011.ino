// =================================================================//
//  SENSOR TEST: SDS011 - Particulate Matter (PM2.5 & PM10)
// =================================================================//

// For this test, I will use the ESP32's second hardware serial port (Serial2).
// This avoids conflicts with the main USB serial port used for programming.
HardwareSerial sdsSerial(2);

// I'm creating a byte array to store the 10-byte data packet from the sensor.
byte sdsBuffer[10];

void setup() {
  // Initialize serial communication with the computer for debugging.
  Serial.begin(9600);
  
  // Initialize serial communication with the SDS011 sensor.
  // It uses a 9600 baud rate. I specify the RX and TX pins for Serial2.
  sdsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX_PIN = 16, TX_PIN = 17
  
  Serial.println("SDS011 sensor test initiated...");
}

void loop() {
  // I check if there is data available from the sensor's serial port.
  if (sdsSerial.available()) {
    // I read the 10-byte packet into my buffer.
    int bytesRead = sdsSerial.readBytes(sdsBuffer, 10);

    // As per the datasheet, I check for the header (0xAA) and tail (0xAB) bytes
    // to ensure I have a complete and valid packet.
    if (sdsBuffer[0] == 0xAA && sdsBuffer[9] == 0xAB) {
      
      // Next, I calculate the checksum to validate data integrity.
      // It's the sum of data bytes 2 through 7.
      byte checksum = sdsBuffer[2] + sdsBuffer[3] + sdsBuffer[4] + sdsBuffer[5] + sdsBuffer[6] + sdsBuffer[7];

      // I compare my calculated checksum with the one sent by the sensor (byte 8).
      if (checksum == sdsBuffer[8]) {
        float pm25 = 0;
        float pm10 = 0;
        
        // Finally, I convert the raw byte data into µg/m³ values using the formula from the datasheet.
        // Formula: (HighByte * 256 + LowByte) / 10.0
        pm25 = ((sdsBuffer[3] * 256.0) + sdsBuffer[2]) / 10.0; 
        pm10 = ((sdsBuffer[5] * 256.0) + sdsBuffer[4]) / 10.0; 

        // I print the final results.
        Serial.print("PM2.5: ");
        Serial.print(pm25);
        Serial.print(" ug/m3  |  ");
        Serial.print("PM10: ");
        Serial.print(pm10);
        Serial.println(" ug/m3");

      } else {
        Serial.println("Checksum error. Data packet is corrupt.");
      }
    }
  }
}