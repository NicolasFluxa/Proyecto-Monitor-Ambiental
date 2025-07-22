# Estación de Monitoreo Ambiental con ESP32 y Firebase
[![Estado del Proyecto: Activo](https://img.shields.io/badge/estado-activo-success.svg)](https://github.com/NicolasFluxa/Proyecto-Monitor-Ambiental) 
[![Tecnología: ESP32](https://img.shields.io/badge/plataforma-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Backend: Firebase](https://img.shields.io/badge/backend-Firebase-orange.svg)](https://firebase.google.com/)

Una estación IoT de código abierto para monitorear la calidad del aire en tiempo real, diseñada para ser replicable y escalable.


---
## Motivación
La calidad del aire que respiramos tiene un impacto directo en nuestra salud y bienestar. Este proyecto nace de la necesidad de crear una herramienta accesible y de bajo costo que permita a cualquier persona medir, entender y tomar conciencia de su entorno, utilizando tecnologías modernas para tomar decisiones informadas.

---
## Características
La estación está diseñada para medir las siguientes variables ambientales:
* **Temperatura y Humedad**
* **Calidad del Aire (Partículas PM2.5 y PM10)**
* **Dióxido de Carbono (CO₂)**
* **Presión Atmosférica**
* **Nivel de Ozono (O₃)**
* Transmisión de datos inalámbrica a través de WiFi.
* Almacenamiento en la nube con Google Firebase Realtime Database.

---
## Arquitectura del Sistema
El flujo de datos del sistema sigue una arquitectura IoT clásica, desde la adquisición hasta la nube:
`[Sensores] -> [ESP32 + Shield] -> [Router WiFi] -> [Firebase Cloud] -> [Usuario Final]`

---
## Componentes y Conexiones
*(Para una descripción técnica detallada de cada componente, consulta el informe completo del proyecto)*

| Componente             | Modelo Específico                               |
| :--------------------- | :---------------------------------------------- |
| **Microcontrolador** | NodeMCU ESP32 WROOM-32                          |
| **Sensor Temp/Hum** | DHT22                                           |
| **Sensor Partículas** | SDS011                                          |
| **Sensor CO₂** | MH-Z19E                                         |
| **Sensor Presión** | GY-BMP280                                       |
| **Sensor Ozono** | MQ-131                                          |
| **Placa de Expansión** | Shield para NodeMCU ESP32                       |
| **Fuente de Poder** | Adaptador de pared 9V o 12V DC, 1A+             |

### Diagrama de Pinout
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

---
## Entorno y Replicación
Para replicar este proyecto, se utilizó el siguiente entorno:
* **Sistema Operativo:** Windows 11
* **IDE:** Arduino IDE v2.3.6
* **Paquete de Placas:** `esp32` by Espressif Systems v2.0.5 o superior.
* **Backend:** Google Firebase - Realtime Database.
* **Librerías:** Consulta la sección de librerías en el informe técnico para ver las versiones exactas.
 

---
## Derechos de Autor y Uso
© 2025, Nicolás Fluxá Morán. Todos los derechos reservados.

Este proyecto y su código fuente son propiedad intelectual de su autor. Se muestran públicamente con fines de demostración y portafolio. No se concede permiso para su uso, copia, modificación o distribución sin el consentimiento expreso y por escrito del autor.

