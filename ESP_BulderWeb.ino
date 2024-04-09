#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "";        // Cambia por tu SSID
const char *password = "";   // Cambia por tu contraseña

ESP8266WebServer server(80);

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
    server.on("/", []() {
        server.send(200, "text/html","<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Mini Web con Cuadrícula de Celdas</title><style>.grid-container {      display: grid;      grid-template-columns: repeat(10, 50px);      grid-template-rows: repeat(15, 50px);      gap: 1px;    }    .grid-item {      background-color: lightgray;      border: 1px solid black;      cursor: pointer;    }  </style></head><body>  <h2>Bulder de 10x15 presas</h2>  <p style='color:blue;'>Manos, una pulsacion</p>  <p style='color:red;'>Pies doble pulsacion rapida</p>  <div class='grid-container'></div>  <script>    const gridContainer = document.querySelector('.grid-container');    for (let i = 0; i < 150; i++) {      const gridItem = document.createElement('div');      gridItem.classList.add('grid-item');      gridItem.setAttribute('data-index', i);      gridItem.addEventListener('click', handleClick);      gridItem.addEventListener('dblclick', handleDoubleClick);      gridContainer.appendChild(gridItem);    }    function handleClick(event) {      const clickedCell = event.target;      const cellIndex = clickedCell.getAttribute('data-index');      const color = clickedCell.style.backgroundColor === 'blue' ? 'lightgray' : 'blue';            clickedCell.style.backgroundColor = color;      sendRequest(cellIndex, color);    }    function handleDoubleClick(event) {      const clickedCell = event.target;      const cellIndex = clickedCell.getAttribute('data-index');      const color = 'red';      clickedCell.style.backgroundColor = color;      sendRequest(cellIndex, color);    }    function sendRequest(cellIndex, color) {      const url = '192.168.1.137/presas?cell=' + cellIndex + '&color=' + color;            fetch(url, { method: 'GET' })        .then(response => {          if (!response.ok) {            throw new Error('Network response was not ok');          }           return response.json();        })        .then(data => {          console.log(data);        })        .catch(error => {          console.error('There has been a problem with your fetch operation:', error);        });    }  </script></body></html>");
    });
    server.on("/presas", HTTP_GET, handleParams);
    server.begin();
    Serial.println("Servidor iniciado");
}

void handleParams() {
  String message = "Parámetros GET recibidos:\n";
  for (int i = 0; i < server.args(); i++) {
    message += "kk " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
  Serial.println(message);
  //// NEOPIXEL
}
void loop() {
    server.handleClient();
}
