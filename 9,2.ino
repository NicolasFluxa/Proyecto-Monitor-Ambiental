// =================================================================//
//  CODIGO FINAL - ESTACION AMBIENTAL v9.1 - LECTURA POR GRUPOS     //
// =================================================================//

// --- LIBRERÍAS ---
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include <DHT.h>
#include <MHZ19.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

// --- CREDENCIALES Y SECRETOS ---
#define WIFI_SSID "Andres_2.4"
#define WIFI_PASSWORD "Alicia97"
#define FIREBASE_HOST "https://estacion-ambiental-mfa-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ZYem1KGG2YFcqWTle4KoW60ou0hv3y3K7rGK0iP0"

// --- CONFIGURACIÓN DE TIEMPO (NTP) ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400; // Offset GMT-4 para Chile
const int   daylightOffset_sec = 0;

// --- OBJETOS DE FIREBASE ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// --- CONFIGURACIÓN DE PINES Y SENSORES ---
// DHT22 (Temp/Hum)
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// SDS011 (Partículas) en Puerto Serie 1
#define SDS_RX_PIN 18
#define SDS_TX_PIN 19
HardwareSerial sdsSerial(1);

// MH-Z19E (CO2) en Puerto Serie 2
#define MHZ_RX_PIN 17
#define MHZ_TX_PIN 16
HardwareSerial mhzSerial(2);
MHZ19 myMHZ19;

// BMP280 (Presión) en I2C
Adafruit_BMP280 bmp;

// Sensores Analógicos
const int o3SensorPin = 35; // MQ-131
const int coSensorPin = 34; // MQ-7

// --- VARIABLES GLOBALES PARA ALMACENAR LECTURAS ---
float ultima_temp = 0.0, ultima_hum = 0.0, ultima_pres = 0.0;
float ultima_pm25 = 0.0, ultima_pm10 = 0.0;
int ultima_co2 = 0, ultimo_o3_raw = 0, ultimo_co_raw = 0;

// --- TEMPORIZADORES PARA LECTURA POR GRUPOS ---
unsigned long timerGrupo1 = 0; // Para sensores analógicos (10s)
unsigned long timerGrupo2 = 0; // Para sensores de clima (30s)
unsigned long timerEnvioFirebase = 0; // Para leer CO2 y enviar todo (60s)


// --- OYENTE DEDICADO PARA EL SENSOR SDS011 ---
void escucharSDS011() {
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
          ultima_pm25 = ((sdsBuffer[3] * 256.0) + sdsBuffer[2]) / 10.0;
          ultima_pm10 = ((sdsBuffer[5] * 256.0) + sdsBuffer[4]) / 10.0;
        }
      }
    }
  }
}

// --- FUNCIÓN DE INICIALIZACIÓN ---
void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando Estacion Ambiental (Sistema Completo)...");

  // Conexión a WiFi y sincronización de hora
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
  Serial.println("\nWiFi Conectado!");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Sincronizando hora...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) { Serial.print("."); delay(1000); }
  Serial.println("\n Hora Sincronizada!");

  // Inicialización de Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("- Firebase... OK");

  // Inicialización de todos los sensores
  dht.begin();
  sdsSerial.begin(9600, SERIAL_8N1, SDS_RX_PIN, SDS_TX_PIN);
  mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX_PIN, MHZ_TX_PIN);
  myMHZ19.begin(mhzSerial);
  if (!bmp.begin(0x76)) { Serial.println("! Error: Sensor BMP280 no encontrado!"); }
  Serial.println("- Todos los sensores inicializados.");
  
  Serial.println("\n--- INICIANDO CICLO DE MONITOREO ---");
}

// --- BUCLE PRINCIPAL ---
void loop() {
  // El oyente del SDS011 se ejecuta en cada ciclo para no perder datos
  escucharSDS011();

  // --- Grupo 1: Sensores Analógicos (cada 10 segundos) ---
  if (millis() - timerGrupo1 > 10000) {
    timerGrupo1 = millis();
    ultimo_o3_raw = analogRead(o3SensorPin);
    ultimo_co_raw = analogRead(coSensorPin);
    Serial.println("[Lectura Grupo 1] Ozono y CO actualizados.");
  }

  // --- Grupo 2: Sensores de Clima (cada 30 segundos) ---
  if (millis() - timerGrupo2 > 30000) {
    timerGrupo2 = millis();
    ultima_temp = dht.readTemperature();
    ultima_hum = dht.readHumidity();
    ultima_pres = bmp.readPressure() / 100.0F;
    Serial.println("[Lectura Grupo 2] Clima actualizado.");
  }

  // --- Grupo 3: Sensor CO2 y Envío a Firebase (cada 60 segundos) ---
  if (Firebase.ready() && (millis() - timerEnvioFirebase > 60000)) {
    timerEnvioFirebase = millis();
    Serial.println("-----------------------------------------");
    Serial.println("[ENVIO] Leyendo CO2 y enviando paquete a Firebase...");
    
    ultima_co2 = myMHZ19.getCO2();
    
    FirebaseJson json;
    
    // Se arma el paquete JSON con las últimas lecturas de cada variable
    if (!isnan(ultima_temp)) json.set("temperatura", String(ultima_temp, 2));
    if (!isnan(ultima_hum)) json.set("humedad", String(ultima_hum, 2));
    if (ultima_pm25 > 0) json.set("pm25", String(ultima_pm25, 2));
    if (ultima_pm10 > 0) json.set("pm10", String(ultima_pm10, 2));
    if (ultima_co2 > 0) json.set("co2", String(ultima_co2));
    if (!isnan(ultima_pres)) json.set("presion", String(ultima_pres, 2));
    json.set("ozono_crudo", String(ultimo_o3_raw));
    json.set("co_crudo", String(ultimo_co_raw));
    
    json.set("timestamp/.sv", "timestamp");

    String path = "/medidor_1/lecturas"; // Path modificado para el Medidor 1
    if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
      Serial.println("--> Paquete enviado a Firebase con exito!");
    } else {
      Serial.println("--> Error al enviar paquete a Firebase: " + fbdo.errorReason());
    }
  }
}