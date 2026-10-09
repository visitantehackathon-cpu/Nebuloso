import os
try:
    from dotenv import load_dotenv
    load_dotenv()
except ImportError:
    pass

# ==========================================
# Configuración del Servidor UDP Local
# ==========================================
UDP_HOST = os.getenv("UDP_HOST", "0.0.0.0")  # Escuchar en todas las interfaces de red
UDP_PORT = int(os.getenv("UDP_PORT", "1700"))  # Puerto UDP estándar de Semtech Packet Forwarder

# ==========================================
# Destino de Datos (Azure / Firebase / Ambos)
# ==========================================
# Opciones: 'azure' (predeterminado), 'firebase', 'both', 'none'
DATA_DESTINATION = os.getenv("DATA_DESTINATION", "azure").lower().strip()

# ==========================================
# Configuración de Microsoft Azure
# ==========================================
# Modo de Azure:
#  - 'iot_hub'      : Enviar como telemetría a Azure IoT Hub (D2C REST)
#  - 'rest_webhook' : Enviar como HTTP POST JSON a Azure Function / App Service / Logic Apps
#  - 'event_hub'    : Enviar a Azure Event Hubs
AZURE_MODE = os.getenv("AZURE_MODE", "iot_hub").lower().strip()

# 1. Opción Azure IoT Hub:
# Puedes usar la cadena de conexión completa del dispositivo (Device Connection String)
# Ejemplo: "HostName=mi-hub.azure-devices.net;DeviceId=Dispositivo_1;SharedAccessKey=xxx="
AZURE_IOT_HUB_CONNECTION_STRING = os.getenv("AZURE_IOT_HUB_CONNECTION_STRING", "")
# O configurar las variables de forma individual:
AZURE_IOT_HUB_NAME = os.getenv("AZURE_IOT_HUB_NAME", "")
AZURE_DEVICE_ID = os.getenv("AZURE_DEVICE_ID", "Dispositivo_1")
AZURE_DEVICE_KEY = os.getenv("AZURE_DEVICE_KEY", "")

# 2. Opción Azure Function / REST Webhook / API:
# Ejemplo: "https://mi-app.azurewebsites.net/api/telemetria"
AZURE_REST_ENDPOINT_URL = os.getenv("AZURE_REST_ENDPOINT_URL", "")
AZURE_REST_API_KEY = os.getenv("AZURE_REST_API_KEY", "")

# 3. Opción Azure Event Hubs:
AZURE_EVENT_HUB_NAMESPACE = os.getenv("AZURE_EVENT_HUB_NAMESPACE", "")
AZURE_EVENT_HUB_NAME = os.getenv("AZURE_EVENT_HUB_NAME", "")
AZURE_EVENT_HUB_SAS_KEY = os.getenv("AZURE_EVENT_HUB_SAS_KEY", "")
AZURE_EVENT_HUB_SAS_POLICY = os.getenv("AZURE_EVENT_HUB_SAS_POLICY", "RootManageSharedAccessKey")

# ==========================================
# Configuración de Firebase (Legacy / Opcional)
# ==========================================
# URL de Firebase Realtime Database
FIREBASE_DATABASE_URL = os.getenv("FIREBASE_DATABASE_URL", "https://datosbase111-default-rtdb.firebaseio.com/")
# Token de Base de Datos / Database Secret
FIREBASE_AUTH_TOKEN = os.getenv("FIREBASE_AUTH_TOKEN", "iFy4c5w94E0707O079dKcDxojPLnqgceFCy2zr6U")
# Nodo en Realtime Database donde se almacenan las mascotas
FIREBASE_RTDB_NODE = os.getenv("FIREBASE_RTDB_NODE", "Mascota")
# Destino en Firebase: 'realtime', 'firestore', o 'both'
FIREBASE_TARGET = os.getenv("FIREBASE_TARGET", "realtime")
# Ruta al archivo de credenciales JSON
FIREBASE_CREDENTIALS_PATH = os.getenv("FIREBASE_CREDENTIALS_PATH", "serviceAccountKey.json")
FIRESTORE_COLLECTION = os.getenv("FIRESTORE_COLLECTION", "estaciones_lora")

# ==========================================
# Dispositivos LoRaWAN Conocidos (Modo ABP)
# ==========================================
KNOWN_DEVICES = {
    # Ejemplo para el Collar (lock.ino / mat2.ino con Dragino LA66 en modo ABP):
    "26011122": {
        "name": "collar_monitoreo_1",
        "appskey": "2b7e151628aed2a6abf7158809cf4f3c",
        "tipo": "collar_bme_max_gps"
    }
}

