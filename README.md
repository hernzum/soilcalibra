# Proyecto de Calibración del Sensor de Humedad del Suelo

Este proyecto permite calibrar un sensor de humedad del suelo utilizando un ESP8266 y Telegram para notificaciones y comandos. Los datos de humedad se almacenan en el sistema de archivos SPIFFS para su posterior análisis.

## Componentes

- D1 Mini ESP8266 (o cualquier otra placa compatible con ESP8266)
- Sensor de humedad del suelo
- Fuente de alimentación (si es necesario)
- Cables de conexión

## Librerías Utilizadas

Este proyecto utiliza las siguientes librerías de Arduino. Asegúrate de instalarlas antes de cargar el código en tu ESP8266.

- [ESP8266WiFi](https://github.com/esp8266/Arduino)
- [WiFiManager](https://github.com/tzapu/WiFiManager)
- [UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [FS](https://github.com/esp8266/Arduino/tree/master/libraries/FS)

## Conexiones

Conecta los componentes de la siguiente manera:

1. **Sensor de Humedad del Suelo:**
   - VCC a 3.3V o 5V (dependiendo del modelo del sensor)
   - GND a GND
   - Salida de señal a A0 (pin analógico del D1 Mini ESP8266)

2. **ESP8266 D1 Mini:**
   - Conecta el ESP8266 a tu ordenador a través de un cable USB para cargar el código y proporcionar alimentación.

## Instalación

1. **Configurar el IDE de Arduino:**
   - Asegúrate de tener la última versión del IDE de Arduino.
   - Agrega el soporte para placas ESP8266 desde el Administrador de Placas (`https://arduino.esp8266.com/stable/package_esp8266com_index.json`).

2. **Instalar Librerías:**
   - Abre el IDE de Arduino.
   - Ve a `Sketch -> Include Library -> Manage Libraries...`
   - Busca e instala las siguientes librerías:
     - `ESP8266WiFi`
     - `WiFiManager`
     - `UniversalTelegramBot`
     - `ArduinoJson`
     - `FS`

3. **Cargar el Código:**
   - Copia y pega el scódigo en el IDE de Arduino:
   - Configura el IDE de Arduino para usar la placa `LOLIN(WEMOS) D1 R2 & mini` (u otra compatible con ESP8266).
   - Carga el código en tu ESP8266.

## Instrucciones de Calibración

1. **Inicializar el Sistema:**
   - Una vez que el ESP8266 esté conectado y el código cargado, se conectará automáticamente a la red WiFi utilizando WiFiManager.
   - Abre el Serial Monitor (115200 baudios) para verificar la conexión.

2. **Inicio de la Calibración:**
   - El bot de Telegram enviará un mensaje indicando que la calibración ha comenzado.
   - El ESP8266 tomará 20 muestras del sensor de humedad, calculará la media y guardará el valor en un archivo en SPIFFS.

3. **Ajuste del Nivel de Humedad:**
   - Después de cada medición, el bot de Telegram solicitará ajustar el nivel de humedad del suelo (por ejemplo, añadiendo agua) para la siguiente calibración.
   - Una vez ajustado el nivel de humedad, envía el comando `/continuar` al bot de Telegram para proceder con la siguiente calibración.

4. **Repetición del Proceso:**
   - El proceso se repetirá cada 60 segundos hasta que decidas detenerlo desconectando el ESP8266.

## Análisis de Datos

- Los datos de calibración se guardan en el archivo `calibration_data.txt` en el sistema de archivos SPIFFS del ESP8266.
- Puedes descargar el archivo de datos para su análisis posterior utilizando un cliente FTP o una herramienta similar compatible con SPIFFS.

## Notas Adicionales

- Asegúrate de tener una conexión a internet estable durante el proceso de calibración.
- Verifica que los componentes estén correctamente conectados para evitar lecturas incorrectas del sensor de humedad.

---

Con estos pasos, deberías poder calibrar tu sensor de humedad del suelo y almacenar los datos de calibración para su análisis posterior. Si tienes alguna pregunta o encuentras algún problema, no dudes en contactar.
