/*******************************************************************
    Sketch para calibrar el sensor de humedad del suelo.
    El código toma 20 muestras, calcula la media y guarda los datos
    en un archivo para su posterior análisis. Además, envía 
    notificaciones a través de Telegram para guiar el proceso de calibración.

    Componentes:
    - D1 Mini ESP8266 (u otro modelo compatible de ESP8266)
    - Sensor de humedad del suelo

    Utiliza:
    - WiFiManager para la configuración de la red WiFi.
    - UniversalTelegramBot para interactuar con el usuario a través de Telegram.
    - SPIFFS para guardar los datos de calibración.

    Autor: Nito
    Versión: 1.1
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <FS.h>

// Token de autenticación del bot de Telegram y ID del chat
#define BOT_TOKEN "7232676749:AAF_2nxG2fgiEXRYMtMc7kC3dcPA3P8KhdA"
#define CHAT_ID "18620566"

// Configuración del cliente seguro y del bot de Telegram
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

const int moistPin = A0; // Pin analógico conectado al sensor de humedad
const int numSamples = 20; // Número de muestras a tomar para la calibración
const char* filename = "/calibration_data.txt"; // Nombre del archivo para guardar los datos

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Configura el WiFi utilizando WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  Serial.println("Conectado a WiFi");
  secured_client.setTrustAnchors(&cert); // Establece el certificado de confianza para la conexión segura

  Serial.print("\nWiFi conectado. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicia el sistema de archivos SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Error al montar el sistema de archivos");
    return;
  }

  // Envía un mensaje inicial al iniciar el bot
  bot.sendMessage(CHAT_ID, "Iniciando calibración del sensor de humedad. Por favor, sigue las instrucciones.", "");
  delay(2000); // Esperar 2 segundos antes de empezar
}

void loop() {
  calibrarSensorDeHumedad();
  delay(60000); // Esperar 60 segundos antes de la siguiente calibración
}

// Función para calibrar el sensor de humedad del suelo
void calibrarSensorDeHumedad() {
  int promedioHumedad = obtenerPromedioHumedad(numSamples);
  String mensaje = "Calibración completada. El valor medio de la humedad es: " + String(promedioHumedad);
  bot.sendMessage(CHAT_ID, mensaje, "");

  // Guardar el valor de humedad en el archivo
  guardarHumedadEnArchivo(promedioHumedad);

  // Solicitar al usuario que ajuste el nivel de humedad para la siguiente calibración
  bot.sendMessage(CHAT_ID, "Por favor, ajusta el nivel de humedad para la siguiente calibración y presiona el botón para continuar.", "");

  // Esperar hasta que se reciba un comando de Telegram para continuar
  esperarComandoContinuar();
}

// Función para obtener el promedio de humedad del suelo
int obtenerPromedioHumedad(int muestras) {
  long total = 0;
  for (int i = 0; i < muestras; i++) {
    total += analogRead(moistPin);
    delay(50); // Pequeña demora entre muestras
  }
  int promedio = total / muestras;
  return promedio;
}

// Función para guardar el valor de humedad en un archivo
void guardarHumedadEnArchivo(int humedad) {
  File file = SPIFFS.open(filename, "a");
  if (!file) {
    Serial.println("Error al abrir el archivo para escribir");
    return;
  }
  file.println(humedad);
  file.close();
}

// Función para esperar un comando de Telegram para continuar
void esperarComandoContinuar() {
  bool continuar = false;
  while (!continuar) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    for (int i = 0; i < numNewMessages; i++) {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      if (text.equals("/continuar")) {
        continuar = true;
        bot.sendMessage(chat_id, "Continuando con la calibración.", "");
        delay(2000); // Esperar 2 segundos antes de continuar
      } else {
        bot.sendMessage(chat_id, "Comando inválido. Usa /continuar para proceder.", "");
      }
    }
    delay(1000); // Esperar 1 segundo antes de verificar nuevamente los mensajes
  }
}
