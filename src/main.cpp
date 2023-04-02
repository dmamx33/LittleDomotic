#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Create an instance of the web server
WebServer server(80);

// Set initial count and LED state
int count = 0;
bool redLedState = false;
bool greenLedState = false;

void handleRoot() {
  // Load the index.html file
  File file = SPIFFS.open("/index.html", "r");
  
  // Send the file content to the client
  server.streamFile(file, "text/html");
  file.close();
}

void handleButtonPress() {
  // Increment the count
  count++;
  
  // Update the LED states
  redLedState = (count == 0);
  greenLedState = (count > 0);
  
  // Send the new count value and LED states to the client
  server.send(200, "text/plain", String(count));
}

void handleButtonHold() {
  // Decrement the count
  count--;
  
  // Update the LED states
  redLedState = (count == 0);
  greenLedState = (count > 0);
  
  // Send the new count value and LED states to the client
  server.send(200, "text/plain", String(count));
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  // Mount the SPIFFS file system
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");
  
  // Configure the LED pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(25, OUTPUT);
  
  // Set the initial LED states
  digitalWrite(LED_BUILTIN, redLedState ? HIGH : LOW);
  digitalWrite(25, greenLedState ? HIGH : LOW);
  
  // Handle requests to the root URL
  server.on("/", HTTP_GET, handleRoot);
  
  // Handle button press and hold events
  server.on("/increment", HTTP_GET, handleButtonPress);
  server.on("/decrement", HTTP_GET, handleButtonHold);
  
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
  
  // Toggle the green LED state every 500 milliseconds
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis >= 500) {
    greenLedState = !greenLedState;
    digitalWrite(25, greenLedState ? HIGH : LOW);
    lastMillis = millis();
  }
  
  // Update the red LED state if necessary
  digitalWrite(LED_BUILTIN, redLedState ? HIGH : LOW);
}

// #include <Arduino.h>
// #include <WiFi.h>
// #include <SPIFFS.h>

// #include <ESPAsyncWebServer.h>

// const char* ssid = "FRITZ!Box 6591 Cable SW";
// const char* password = "62407078731195560963";

// AsyncWebServer server(80);

// int counter = 0;

// void setup() {
//   Serial.begin(115200);
// SPIFFS.begin(true);

//   // Conectar a la red Wi-Fi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Conectando a WiFi...");
//   }
//   Serial.println("Conexión WiFi establecida");

//   // Ruta para manejar la petición del botón
//   server.on("/button", HTTP_POST, [](AsyncWebServerRequest *request){
//     counter++;
//     request->send(200, "text/plain", String(counter));
//   });

//   // Ruta para obtener el contador actual
//   server.on("/counter", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(200, "text/plain", String(counter));
//   });

//   // Servir el archivo HTML
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(SPIFFS, "/index.html");
//   });

//   // Iniciar el servidor
//   server.begin();
// }

// void loop() {

//   Serial.println("Estoy vivo");
//   delay(1000);  
// }