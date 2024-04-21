#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "";        // Cambia por tu SSID
const char *password = "";   // Cambia por tu contraseña
#define PIN        2 
#define NUMPIXELS 30


String web = "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Bulder web</title><style>.grid-container {      display: grid;      grid-template-columns: repeat(10, 50px);      grid-template-rows: repeat(15, 50px);      gap: 1px;    }    .grid-item {      background-color: lightgray;      border: 1px solid black;      cursor: pointer;    }  </style></head><body>  <h2>Bulder de 10x15 presas  ----- version 1.1</h2>  <p style='color:blue;'>Manos, una pulsacion</p>  <p style='color:red;'>Pies, doble pulsación</p>  <div class='grid-container'></div>  <script>    const gridContainer = document.querySelector('.grid-container');    for (let i = 0; i < 150; i++) {      const gridItem = document.createElement('div');      gridItem.classList.add('grid-item');      gridItem.setAttribute('data-index', i);      gridItem.addEventListener('click', handleClick);      gridItem.addEventListener('dblclick', handleDoubleClick);      gridContainer.appendChild(gridItem);    }    function handleClick(event) {      const clickedCell = event.target;      const cellIndex = clickedCell.getAttribute('data-index');      const color = clickedCell.style.backgroundColor === 'blue' ? 'lightgray' : 'blue';            clickedCell.style.backgroundColor = color;      sendRequest(cellIndex, color);    }    function handleDoubleClick(event) {      const clickedCell = event.target;      const cellIndex = clickedCell.getAttribute('data-index');      const color = 'red';      clickedCell.style.backgroundColor = color;      sendRequest(cellIndex, color);    }    function sendRequest(cellIndex, color) {      const url = '/presas?presa=' + cellIndex + '&tipo=' + color;            fetch(url, { method: 'GET' })        .then(response => {          if (!response.ok) {            throw new Error('Network response was not ok');          }           return response.json();        })        .then(data => {          console.log(data);        })        .catch(error => {console.log('hay un error capullo:', error);        });    }  </script></body></html>";
ESP8266WebServer server(80);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Esperar a que se conecte
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Conectado a ");
    Serial.println(ssid);
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

///VERSION WEB CELDAS 1.0
    server.on("/", []() {server.send(200, "text/html", web);});
    server.on("/presas", HTTP_GET, handleParams);
    server.begin();
    Serial.println("Servidor iniciado");

    // NEOPIXEL
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear();
}

void handleParams() {
  int presa;
  int color;

    Serial.println("mensaje ");
      if (server.argName(0).equals("presa")) {
        presa = server.arg(0).toInt();
        Serial.println("PRESA="+ server.arg(0) + "\n" );
      } else {
        Serial.println("path invalido =" + server.argName(0) );
      }
      if (server.argName(1).equals("tipo")) {
        Serial.println("TIPO="+ server.arg(1) + "\n" );
            if (server.arg(1).equals("red")) {
                color =  pixels.Color(255, 0, 0);  // Enciende el pixel en rojo
            } else if (server.arg(1).equals("blue")) {
                color = pixels.Color(0, 0, 255);  // Enciende el pixel en azul
            } else if (server.arg(1).equals("lightgray")) {
                color = pixels.Color(0, 0, 0);  // Apaga 
            }
      }
      else {
        Serial.println("path invalido 0" + server.argName(0)  );
      }

  //// NEOPIXEL
  //pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(valor_entero, color);
  pixels.show();  

  server.send(200,"application/json","");
}
void loop() {

    server.handleClient();
}
