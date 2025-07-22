#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include <DHT.h>
#include <MHZ19.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define FIREBASE_HOST "https://your-project-id.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 0;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SDS_RX_PIN 18
#define SDS_TX_PIN 19
HardwareSerial sdsSerial(1);

const int o3SensorPin = 35;

#define MHZ_RX_PIN 17
#define MHZ_TX_PIN 16

float latest_pm25 = 0.0;
float latest_pm10 = 0.0;
unsigned long sendDataPrevMillis = 0;

void sds011_listener() {
  static byte sdsBuffer[10];
  static int sdsIndex = 0;

  while (sdsSerial.available()) {
    byte c = sdsSerial.read();
    if (sdsIndex == 0 && c != 0xAA) {
      continue;
    }
    if (sdsIndex < 10) {
      sdsBuffer[sdsIndex++] = c;
    }
    if (sdsIndex == 10) {
      sdsIndex = 0;
      if (sdsBuffer[9] == 0xAB) {
        byte checksum = sdsBuffer[2] + sdsBuffer[3] + sdsBuffer[4] + sdsBuffer[5] + sdsBuffer[6] + sdsBuffer[7];
        if (checksum == sdsBuffer[8]) {
          latest_pm25 = ((sdsBuffer[3] * 256.0) + sdsBuffer[2]) / 10.0;
          latest_pm10 = ((sdsBuffer[5] * 256.0) + sdsBuffer[4]) / 10.0;
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n==============================================");
  Serial.println("Initializing Environmental Station (Final Version)");
  Serial.println("==============================================");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
  Serial.println("\nWiFi Connected!");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Syncing time...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) { Serial.print("."); delay(1000); }
  Serial.println("\nTime Synchronized!");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("- Firebase... OK");

  dht.begin();
  sdsSerial.begin(9600, SERIAL_8N1, SDS_RX_PIN, SDS_TX_PIN);
  Serial.println("- Active sensors initialized.");

  Serial.println("\n--- STARTING READINGS AND CLOUD UPLOAD ---");
}

void loop() {
  sds011_listener();

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 60000)) {
    sendDataPrevMillis = millis();
    Serial.println("-----------------------------------------");
    Serial.println("Reading sensors and sending to Firebase...");
    FirebaseJson json;
    
    float temp_dht = dht.readTemperature();
    float hum_dht = dht.readHumidity();
    int o3_raw = analogRead(o3SensorPin);
    
    if (!isnan(temp_dht)) json.set("temperature", String(temp_dht, 2));
    if (!isnan(hum_dht)) json.set("humidity", String(hum_dht, 2));
    if (latest_pm25 > 0) json.set("pm25", String(latest_pm25, 2));
    if (latest_pm10 > 0) json.set("pm10", String(latest_pm10, 2));
    json.set("ozone_raw", String(o3_raw));
    
    json.set("timestamp/.sv", "timestamp");

    String path = "/final_readings";
    if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
      Serial.println("Successfully sent data to Firebase!");
    } else {
      Serial.println("Error sending data to Firebase: " + fbdo.errorReason());
    }
  }
}