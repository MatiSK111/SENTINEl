#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <RF24.h>

#define RXPin 16  // Pin RX del GPS conectado al pin 16 del ESP32
#define TXPin 17  // Pin TX del GPS conectado al pin 17 del ESP32

RF24 radio(4, 5); // CE, CSN
char receivedText[50]; // Almacenar el mensaje recibido
const char* ssid = "SENTINEL";                             // Nombre de tu red Wi-Fi
const char* password = "12345678";                      // Contraseña de tu red Wi-Fi
HardwareSerial GPSSerial(1);  // Utiliza Serial1 para comunicarse con el módulo GPS
TinyGPSPlus gps;


void setup() {
  Serial.begin(9600);                                 // Inicia la comunicación serie a una velocidad de 115200 bps

  WiFi.softAP(ssid, password);                          // Inicia el modo de punto de acceso Wi-Fi con el nombre de red y contraseña especificados
  
  IPAddress myIP = WiFi.softAPIP();                      // Obtiene la dirección IP asignada al punto de acceso
  Serial.print("Dirección IP del punto de acceso: ");
  Serial.println(myIP);                                 // Imprime la dirección IP del punto de acceso en el monitor serial
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0E1LL); // Dirección del emisor
  radio.setPALevel(RF24_PA_LOW); // Establecer la potencia de recepción (ajústala según sea necesario)
  radio.startListening();
}

void loop() {
  // Tu código aquí
   //while (GPSSerial.available() > 0) {
   // gps.encode(GPSSerial.read());
   // if (gps.location.isUpdated()) {
    //  double latitude = gps.location.lat();
     // double longitude = gps.location.lng();
      
     // Serial.print("Latitud: ");
     // Serial.println(latitude, 6); // 6 decimales de precisión
     // Serial.print("Longitud: ");
     // Serial.println(longitude, 6); // 6 decimales de precisión
    //}
 // }
  if (radio.available()) {
    radio.read(&receivedText, sizeof(receivedText)); // Lee el mensaje recibido
    Serial.println("Mensaje recibido:");
    Serial.println(receivedText); // Imprime el mensaje en el puerto serie

    // Si el mensaje contiene datos de ubicación GPS
    if (strstr(receivedText, "Latitud:") && strstr(receivedText, "Longitud:")) {
      Serial.println("Datos de ubicación GPS recibidos:");
      Serial.println(receivedText); // Imprime los datos de ubicación GPS en el puerto serie
    }
  }
}
