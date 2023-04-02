#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>

#include <ESPAsyncWebServer.h>

const char* ssid = "FRITZ!Box 6591 Cable SW";
const char* password = "62407078731195560963";

AsyncWebServer server(80);

int counter = 0;

void setup() {
  Serial.begin(115200);
SPIFFS.begin(true);

  // Conectar a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión WiFi establecida");

  // Ruta para manejar la petición del botón
  server.on("/button", HTTP_POST, [](AsyncWebServerRequest *request){
    counter++;
    request->send(200, "text/plain", String(counter));
  });

  // Ruta para obtener el contador actual
  server.on("/counter", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(counter));
  });

  // Servir el archivo HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  // Iniciar el servidor
  server.begin();
}

void loop() {

  Serial.println("Estoy vivo");
  delay(1000);  
}