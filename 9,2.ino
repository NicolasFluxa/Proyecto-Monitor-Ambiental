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

#define MHZ_RX_PIN 17
#define MHZ_TX_PIN 16
HardwareSerial mhzSerial(2);
MHZ19 myMHZ19;

Adafruit_BMP280 bmp;

const int o3SensorPin = 35;
const int coSensorPin = 34;

float latest_temp = 0.0, latest_hum = 0.0, latest_pres = 0.0;
float latest_pm25 = 0.0, latest_pm10 = 0.0;
int latest_co2 = 0, latest_o3_raw = 0, latest_co_raw = 0;

unsigned long timerGroup1 = 0;
unsigned long timerGroup2 = 0;
unsigned long timerFirebaseSend = 0;

void sds011_listener() {
  static byte sdsBuffer[10];
  static int sdsIndex = 0;
  while (sdsSerial.available()) {
    byte c = sdsSerial.read();
    if (sdsIndex == 0 && c != 0xAA) continue;
    if (sdsIndex < 10) sdsBuffer[sdsIndex++] = c;
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
  Serial.println("\nInitializing Environmental Station (Full System)...");

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
  mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX_PIN, MHZ_TX_PIN);
  myMHZ19.begin(mhzSerial);
  if (!bmp.begin(0x76)) { Serial.println("! Error: BMP280 sensor not found!"); }
  Serial.println("- All sensors initialized.");
  
  Serial.println("\n--- MONITORING CYCLE STARTED ---");
}

void loop() {
  sds011_listener();

  if (millis() - timerGroup1 > 10000) {
    timerGroup1 = millis();
    latest_o3_raw = analogRead(o3SensorPin);
    latest_co_raw = analogRead(coSensorPin);
  }

  if (millis() - timerGroup2 > 30000) {
    timerGroup2 = millis();
    latest_temp = dht.readTemperature();
    latest_hum = dht.readHumidity();
    latest_pres = bmp.readPressure() / 100.0F;
  }

  if (Firebase.ready() && (millis() - timerFirebaseSend > 60000)) {
    timerFirebaseSend = millis();
    
    latest_co2 = myMHZ19.getCO2();
    
    FirebaseJson json;
    
    if (!isnan(latest_temp)) json.set("temperatura", String(latest_temp, 2));
    if (!isnan(latest_hum)) json.set("humedad", String(latest_hum, 2));
    if (latest_pm25 > 0) json.set("pm25", String(latest_pm25, 2));
    if (latest_pm10 > 0) json.set("pm10", String(latest_pm10, 2));
    if (latest_co2 > 0) json.set("co2", String(latest_co2));
    if (!isnan(latest_pres)) json.set("presion", String(latest_pres, 2));
    json.set("ozono_crudo", String(latest_o3_raw));
    json.set("co_crudo", String(latest_co_raw));
    
    json.set("timestamp/.sv", "timestamp");

    String path = "/medidor_1";
    if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
      Serial.println("--> Data package successfully sent to Firebase!");
    } else {
      Serial.println("--> Error sending data package to Firebase: " + fbdo.errorReason());
    }
  }
}
