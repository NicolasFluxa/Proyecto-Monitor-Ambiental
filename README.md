# Estación de Monitoreo Ambiental con ESP32 y Firebase

## 1. Descripción del Proyecto
Este es un proyecto de ingeniería de Internet de las Cosas (IoT) que consiste en una estación de monitoreo ambiental integral. El sistema utiliza un microcontrolador ESP32 para recopilar datos de un conjunto completo de sensores y los envía en tiempo real a una base de datos en la nube (Google Firebase) para su almacenamiento y posterior análisis.

El objetivo principal es crear un dispositivo funcional y robusto, documentando cada fase del proceso, desde la selección de componentes hasta la solución de problemas, para que el proyecto sea fácilmente replicable en su totalidad.

---

## 2. Características
La estación está diseñada para medir las siguientes variables ambientales:
* **Temperatura y Humedad**
* **Calidad del Aire (Partículas PM2.5 y PM10)**
* **Dióxido de Carbono (CO₂)**
* **Presión Atmosférica**
* **Nivel de Ozono (O₃)**
* **Nivel de Ruido Ambiental**
* Transmisión de datos inalámbrica a través de WiFi.
* Almacenamiento en la nube con Google Firebase Realtime Database.

---

## 3. Componentes y Hardware
| Componente             | Modelo Específico                               | Propósito                                 |
| :--------------------- | :---------------------------------------------- | :---------------------------------------- |
| **Microcontrolador** | NodeMCU ESP32 WROOM-32                          | Cerebro del proyecto, gestiona y envía datos |
| **Sensor Temp/Hum** | DHT22                                           | Medición de temperatura y humedad         |
| **Sensor Partículas** | SDS011                                          | Medición de PM2.5 y PM10                  |
| **Sensor CO₂** | MH-Z19E                                         | Medición de Dióxido de Carbono            |
| **Sensor Presión** | GY-BMP280                                       | Medición de presión atmosférica           |
| **Sensor Ozono** | MQ-131                                          | Medición de O₃ (lectura cruda)            |
| **Sensor Sonido** | KY-037                                          | Medición de nivel de ruido                |
| **Placa de Expansión** | Shield para NodeMCU ESP32                       | Facilita conexiones y gestiona la energía |
| **Fuente de Poder** | Adaptador de pared 9V o 12V DC, 1A (o superior) | Alimenta todo el sistema de forma estable |

---

## 4. Diagrama de Conexiones Final
Esta tabla muestra la distribución de pines para el proyecto completo, con todos los sensores conectados.

| Sensor      | Pin del Sensor | Pin del ESP32 |
| :---------- | :------------- | :------------ |
| **DHT22** | DATA           | D4 (GPIO 4)   |
| **SDS011** | TX             | D19 (GPIO 19) |
|             | RX             | D18 (GPIO 18) |
| **MH-Z19E** | TX             | D16 (GPIO 16) |
|             | RX             | D17 (GPIO 17) |
| **BMP280** | SCL            | D22 (GPIO 22) |
|             | SDA            | D21 (GPIO 21) |
| **MQ-131** | AOUT           | D35 (GPIO 35) |
| **KY-037** | AO             | D34 (GPIO 34) |

---

## 5. Entorno de Desarrollo y Software
Para replicar este proyecto, se utilizó el siguiente entorno:

* **Sistema Operativo:** Windows 11
* **IDE:** Arduino IDE v2.3.6
* **Paquete de Placas:** `esp32` by Espressif Systems v2.0.5 o superior.
* **Backend:** Google Firebase - Realtime Database.

### Librerías de Arduino
Es crucial usar estas versiones específicas para asegurar la compatibilidad.

| Librería                                             | Autor             | Versión Recomendada |
| :--------------------------------------------------- | :---------------- | :------------------ |
| **DHT sensor library** | Adafruit          | 1.4.6               |
| **Firebase Arduino Client Library for ESP8266 & ESP32** | Mobizt            | 4.4.17              |
| **MH-Z19** | Jonathan Dempsey  | 1.5.4               |
| **Adafruit BMP280 Library** | Adafruit          | 2.6.8               |
| **Adafruit BusIO** | Adafruit          | 1.17.2              |
| **Adafruit Unified Sensor** | Adafruit          | 1.1.15              |

---

## 6. Para Replicar el Proyecto
1.  **Montar el Hardware:** Reúne todos los componentes y conéctalos según el diagrama de pinout.
2.  **Configurar el Entorno:** Instala el IDE de Arduino y el paquete de soporte para placas ESP32 (v2.0.5+).
3.  **Instalar Librerías:** Usando el Gestor de Librerías, instala cada una de las librerías listadas.
4.  **Crear Proyecto en Firebase:** Crea un nuevo proyecto y una Realtime Database. Copia la URL y la Clave Secreta.
5.  **Configurar y Subir el Código:** Descarga el código fuente, actualiza las credenciales de WiFi y Firebase, y súbelo al ESP32.
