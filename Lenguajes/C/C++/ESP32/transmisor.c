/// Abrir unicamente en IDE(por librerias y rutas)//
#include <Wire.h>
#include <MAX30105.h>
#include <TinyGPSPlus.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

const int RXPin = 16;
const int TXPin = 17;
const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial gpsPort(2);

const int sdaTemp = 8;
const int sclTemp = 6;
const uint8_t MAX30205_ADDRESS = 0x4F; 
const uint8_t TEMP_REG = 0x00;

const int sdaCardiaco = 4;
const int sclCardiaco = 5;
MAX30105 sensorCardiaco;

typedef struct struct_message {
    int id;            
    long ir;            
    long red;           
    float temperatura;  
    float latitud;
    float longitud;
} struct_message;

struct_message datosAEnviar;

uint8_t direccionReceptor[] = {0x3C, 0xDC, 0x75, 0x6B, 0xCE, 0x34}; 
esp_now_peer_info_t peerInfo;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
    Serial.print("Estado del envio ESP-NOW: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Exito" : "Fallo");
}

float leerTemperaturaMAX30205() {
    for (int i = 0; i < 3; i++) {
        Wire.beginTransmission(MAX30205_ADDRESS);
        Wire.write(TEMP_REG);
        if (Wire.endTransmission() == 0) {
            Wire.requestFrom(MAX30205_ADDRESS, (uint8_t)2);
            if (Wire.available() == 2) {
                uint8_t msb = Wire.read();
                uint8_t lsb = Wire.read();
                int16_t rawTemp = (msb << 8) | lsb;
                return rawTemp * 0.00390625;
            }
        }
        delay(10);
    }
    return -999.0;
}

void setup() {
    Serial.begin(115200);

    gpsPort.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);

    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);

    if (esp_now_init() != ESP_OK) {
        return;
    }
    
    esp_now_register_send_cb(OnDataSent);

    memcpy(peerInfo.peer_addr, direccionReceptor, 6);
    peerInfo.channel = 6;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        return;
    }

    Wire.begin(sdaTemp, sclTemp); 
    Wire.setClock(100000);

    Wire1.begin(sdaCardiaco, sclCardiaco, 400000);
    if (sensorCardiaco.begin(Wire1, I2C_SPEED_FAST)) {
        sensorCardiaco.setup();
    }

    datosAEnviar.id = 1;
}

void loop() {
    while (gpsPort.available() > 0) {
        gps.encode(gpsPort.read());
    }

    datosAEnviar.temperatura = leerTemperaturaMAX30205();
    datosAEnviar.ir = sensorCardiaco.getIR();
    datosAEnviar.red = sensorCardiaco.getRed();

    if (gps.location.isValid()) {
        datosAEnviar.latitud = gps.location.lat();
        datosAEnviar.longitud = gps.location.lng();
    } else {
        datosAEnviar.latitud = 0.0;
        datosAEnviar.longitud = 0.0;
    }

    Serial.print("IR: "); Serial.print(datosAEnviar.ir);
    Serial.print(" | RED: "); Serial.print(datosAEnviar.red);
    Serial.print(" | Temp: "); Serial.print(datosAEnviar.temperatura);
    Serial.print(" | Lat: "); Serial.print(datosAEnviar.latitud, 6);
    Serial.print(" | Lng: "); Serial.println(datosAEnviar.longitud, 6);

    esp_now_send(direccionReceptor, (uint8_t *) &datosAEnviar, sizeof(datosAEnviar));

    delay(2000);
}