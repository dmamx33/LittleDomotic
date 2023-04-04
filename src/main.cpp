#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "FRITZ!Box 6591 Cable SW";
const char* password = "62407078731195560963";

WebServer server(80);

int contador = 0;
int tiempo=0;
const int LED1 = 25;
const int LED2 = 26;
const int LED3 = 27;
const int BUTTON = 33;


boolean BLOCK=false;
boolean COOLING = true;
int ciclo_espera=1;
boolean ledAzulstate = false;

void handleRoot() {
  String html = "<html><head><title>ESP32 Web Server</title></head><body>";
  html += "<h1>Steurung Kuehlung Papiervernichter</h1>";
  html += "<h1 id=\"contador\">Bedienungzeit: " + String(contador) + "</h1>";
  html += "<p><a href=\"/led1on\"><button>Encender LED 1</button></a></p>";
  html += "<p><a href=\"/led1off\"><button>Apagar LED 1</button></a></p>";
  html += "<p><a href=\"/led2on\"><button>Encender LED 2</button></a></p>";
  html += "<p><a href=\"/led2off\"><button>Apagar LED 2</button></a></p>";
  html += "<p><a href=\"/led3on\"><button>Encender LED 3</button></a></p>";
  html += "<p><a href=\"/led3off\"><button>Apagar LED 3</button></a></p>";
  html += "<script>setInterval(function() {"
          "var xhttp = new XMLHttpRequest();"
          "xhttp.onreadystatechange = function() {"
          "if (this.readyState == 4 && this.status == 200) {"
          "document.getElementById(\"contador\").innerHTML = \"Contador: \" + this.responseText;"
          "}"
          "};"
          "xhttp.open(\"GET\", \"/getcontador\", true);"
          "xhttp.send();"
          "}, 1000);</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLED1On() {
  digitalWrite(LED1, HIGH);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleLED1Off() {
  digitalWrite(LED1, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleLED2On() {
  digitalWrite(LED2, HIGH);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");

}

void handleLED2Off() {
  digitalWrite(LED2, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleLED3On() {
  digitalWrite(LED3, HIGH);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleLED3Off() {
  digitalWrite(LED3, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/led1on", handleLED1On);
  server.on("/led1off", handleLED1Off);
  server.on("/led2on", handleLED2On);
  server.on("/led2off", handleLED2Off);
  server.on("/led3on", handleLED3On);
  server.on("/led3off", handleLED3Off);
  server.on("/getcontador", HTTP_GET, []() {
  server.send(200, "text/plain", String(contador));
});

  server.begin();
}

void loop() {
  server.handleClient();

  if(!BLOCK)/// EL motor no está bloqueado
  {

    digitalWrite(LED3,!digitalRead(BUTTON));
    if(!digitalRead(BUTTON))/////Boton apretado
    {
      tiempo++;
    }
    else
    {
        if (tiempo>0)
        {
              if(ciclo_espera >= 150)///Si se cumplen los 15 segundos decrementa el tiempo AN
                {
                tiempo=tiempo-10;
                ciclo_espera = 1;
                }
                else 
                ciclo_espera++; // Decrementa el ciclo una decima de segundo
        }
    }
    
    if(tiempo>=1200)///Se llego a tiempo limite, bloquea motor
      {
        BLOCK=true;
        digitalWrite(LED2,true);
      }
  }
  else /// EL motor está bloqueado
  {
    if(ciclo_espera >= 150)///Si se cumplen los 15 segundos decrementa el tiempo AN
    {
      tiempo=tiempo-10;
      ciclo_espera = 1;
    }
    else 
      ciclo_espera++; // Decrementa el ciclo una decima de segundo
      //Serial.println(ciclo_espera);
      //Serial.println(tiempo);
      //////////////////////////////////////////////////

    if(tiempo<=0)//Tiempo de espera de 30 minutos total tras 2 minutos de operacion
    {
      ciclo_espera=1;
      digitalWrite(LED2,false);
      BLOCK = false;
      Serial.println("Desbloqueo");
    }//////////////////////////////////////////////////

  }

  if(tiempo>10)/////Cooling activo parpadea led azul
  {
    ledAzulstate = !ledAzulstate;
    digitalWrite(LED1,ledAzulstate);
  }
  else
  digitalWrite(LED1,false);
//////////////////////////////////////////////////
  contador = int(tiempo/10);////escribe en el webserver
  delay(100);
}



  /* if (digitalRead(BUTTON) == LOW) {
    contador++;
    digitalWrite(LED3, HIGH);
  }
  else
    digitalWrite(LED3, LOW);
  delay(1000); */