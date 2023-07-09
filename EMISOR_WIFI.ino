#include <WiFi.h>

const char* ssid = "SENTINEL";                             // Nombre de tu red Wi-Fi
const char* password = "12345678";                      // Contraseña de tu red Wi-Fi

void setup() {
  Serial.begin(115200);                                 // Inicia la comunicación serie a una velocidad de 115200 bps

  WiFi.softAP(ssid, password);                          // Inicia el modo de punto de acceso Wi-Fi con el nombre de red y contraseña especificados
  
  IPAddress myIP = WiFi.softAPIP();                      // Obtiene la dirección IP asignada al punto de acceso
  Serial.print("Dirección IP del punto de acceso: ");
  Serial.println(myIP);                                 // Imprime la dirección IP del punto de acceso en el monitor serial
}

void loop() {
  // Tu código aquí
}
