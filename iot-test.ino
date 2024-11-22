/*
IoT Test Sample

Cliente en ESP32 que se comunica a través de HTTP a una API rest

Jorge E. Turner
21 Noviembre 2024
*/

#include "secrets.h"
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Configuración de red
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;


// API endpoint
const char* endpointAPI = "http://192.168.1.69:3000/api/sensores";

//Sensores
//Temperatura y humedad
#define DHT11_PIN 2
DHT dht11(DHT11_PIN, DHT11);

//Nivel de agua
#define WATER_PIN A0

//Definir frecuencia de actualización
//Tiempo en milisegundos
unsigned long lastTime = 0;
// Timer a 10 minutos 600000
// Timer a 5 segundos 5000
unsigned long timerDelay = 5000;


void setup() {

  //Iniciar velocidad de comunicación
  //Para menor frecuencia usar 9600 baudios
  Serial.begin(115200);

  // Iniciar conexión WiFi
  Serial.print("Conectándose...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado a la red");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());  //Ip asignada al ESP32


  dht11.begin();  // inicializar sensor de temperatura
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {

    //Obtener valores de sensores
    float temperature = getTemperature();
    float humidity = getHumidity();
    float waterLevel = getWaterLevel();

    //Construir payload en formato JSON
    //El segundo argumento de String() trunca los decimales a dos
    String payload = "{\"temperature\": "
                     + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2)
                     + ", \"waterLevel\": "
                     + String(waterLevel, 2)
                     + "}";

    Serial.println(payload);


    // Revisar conexión WiFi
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Especificar endpoint
      http.begin(endpointAPI);

      // Configurar headers
      http.addHeader("Content-Type", "application/json");

      // POST
      int httpResponseCode = http.POST(payload);


      // Manejo de errores
      if (httpResponseCode > 0) {
        Serial.println("Response Code: " + String(httpResponseCode));
        String response = http.getString();
        Serial.println("Respuesta: " + response);
      } else {
        Serial.println("Error enviando POST request");
      }

      http.end();  //Terminar conexión
    } else {

      Serial.println("Desconectado. Intentando reconectar...");
      WiFi.reconnect();
    }

    lastTime = millis();
  }
}

float getTemperature() {

  // temperatura en Celcius, para Fahrenheit pasar True como argumento
  float temperature = dht11.readTemperature();

  // Manejo de errores
  if (isnan(temperature)) {
    Serial.println("No se pudo leer la temperatura de DHT11");
    return -1;
  }

  return temperature;
}

float getHumidity() {
  float humidity = dht11.readHumidity();

  // Manejo de errores
  if (isnan(humidity)) {
    Serial.println("No se pudo leer la humedad de DHT11");
    return -1;
  }

  return humidity;
}

float getWaterLevel() {

  int val = analogRead(WATER_PIN);

  //Manejo de errores
  if (isnan(val)) {
    Serial.println("No se pudo leer la humedad de DHT11");
    return -1;
  }
  // Conversion de voltage a valores numéricos
  float nivel_agua = (val / 4095.0) * 3.3;  //ESP32 12-bit ADC a 3.3V

  return nivel_agua;
}
