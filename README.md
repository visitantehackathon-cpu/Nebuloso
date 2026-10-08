# SmartEco Health & Telemetry - Sistema IoT con LoRaWAN y Energía Solar

Solución integral de Internet de las Cosas (IoT) orientada a la recolección, transmisión segura y visualización de datos telemétricos en entornos remotos, alimentada de forma autónoma mediante energía solar.

---

## 1. Descripción General del Proyecto

El sistema permite monitorear variables críticas, tales como métricas biométricas y ambientales a través de sensores especializados, recolectadas mediante nodos inteligentes basados en ESP32 y transmitidas a larga distancia utilizando tecnología LoRaWAN. La plataforma integra un backend robusto y una interfaz web con control de accesos por roles.



## 2. Stack Tecnológico

### Hardware
 -Microcontrolador: ESP32-S3 / WROOM-1
 -Módulo de Comunicación: LoRaWAN LA66
 -Gateway: Gateway Bega 1.1
 -Sensores: MAX30201 / MAX3050 (Sensores biométricos y de -temperatura)
 -Alimentación: Panel Solar ARG (Sistema autónomo)

### Software y Lenguajes
 -C / C++: Desarrollo de firmware, gestión de buses I2C y adquisición de datos en los microcontroladores.

 -JavaScript (.JS): Lógica del backend y componentes interactivos de la interfaz web.
 
 -Python: Scripts de automatización, procesamiento de datos telemétricos y utilidades.
