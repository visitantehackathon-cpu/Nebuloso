#include <esp_now.h>
#include <WiFi.h>



#define LORA_RX_PIN 18  
#define LORA_TX_PIN 17  
HardwareSerial LoRaSerial(1); 


typedef struct struct_message {
    int id;             
    long ir;            
    long red;           
    float temperatura;  
    float latitud;      
    float longitud;     
} struct_message;

struct_message datosRecibidos;
bool nuevoDatoRecibido = false;


#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
#else
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
#endif
    if (len == sizeof(datosRecibidos)) {
        memcpy(&datosRecibidos, incomingData, sizeof(datosRecibidos));
        nuevoDatoRecibido = true;
    }
}


void enviarPorLoRaWAN(struct_message *datos) {
    
    String hexPayload = "";
    uint8_t *bytes = (uint8_t *)datos;
    for (size_t i = 0; i < sizeof(struct_message); i++) {
        char buffer[3];
        sprintf(buffer, "%02X", bytes[i]);
        hexPayload += buffer;
    }

   String comando = "AT+SENDB=0,2," + String(sizeof(struct_message)) + "," + hexPayload;
    
    Serial.print(">>> Enviando a LoRaWAN: ");
    Serial.println(comando);

   
    LoRaSerial.println(comando);
}

void setup() {
  
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== INICIANDO NODO PUENTE ESP-NOW -> LORAWAN ===");

    
    LoRaSerial.begin(9600, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
    Serial.println("Puerto serie con LA66 listo.");

    
    LoRaSerial.println("AT+JOIN");

    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    
    if (esp_now_init() != ESP_OK) {
        Serial.println("ERROR: No se pudo inicializar ESP-NOW");
        return;
    }

    #if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    esp_now_register_recv_cb(OnDataRecv);
    #else
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    #endif

    Serial.println("Listo y esperando datos del collar para enviarlos al Gateway...");
}

void loop() {

    if (LoRaSerial.available()) {
        Serial.write(LoRaSerial.read());
    }


    if (nuevoDatoRecibido) {
        nuevoDatoRecibido = false;

        Serial.println("\n--- [ESP-NOW] Paquete recibido del Collar ---");
        Serial.print("ID Dispositivo: "); Serial.println(datosRecibidos.id);
        Serial.print("IR: ");            Serial.println(datosRecibidos.ir);
        Serial.print("Red: ");           Serial.println(datosRecibidos.red);
        Serial.print("Temperatura: ");   Serial.print(datosRecibidos.temperatura); Serial.println(" °C");
        Serial.print("Latitud: ");       Serial.println(datosRecibidos.latitud, 6);
        Serial.print("Longitud: ");      Serial.println(datosRecibidos.longitud, 6);

 
        enviarPorLoRaWAN(&datosRecibidos);
    }
}