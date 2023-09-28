#include <WiFi.h>
#include <math.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN
const byte identificacion[6] = "00001";
#define RXPin 16  // Pin RX del GPS conectado al pin 16 del ESP32
#define TXPin 17  // Pin TX del GPS conectado al pin 17 del ESP32

const int PIN = 2;                                      // Pin utilizado para controlar una salida digital
const char* ssid = "SENTINEL";                          // Nombre de tu red Wi-Fi
const char* password = "12345678";                      // Contraseña de tu red Wi-Fi
const int REFERENCE_RSSI = -50;                          // Intensidad de señal RSSI a una distancia conocida
const float REFERENCE_DISTANCE = 1.0;                    // Distancia conocida en metros para la intensidad de señal RSSI de referencia
const int pinZumbador = 12;                               // Pin utilizado para controlar el zumbador
HardwareSerial GPSSerial(1);  // Utiliza Serial1 para comunicarse con el módulo GPS
TinyGPSPlus gps;
int i=0;
void setup() {
  
  Serial.begin(9600);                                 // Inicia la comunicación serie a una velocidad de 115200 bps
  GPSSerial.begin(9600, SERIAL_8N1, RXPin, TXPin);
  pinMode(PIN, OUTPUT);                                 // Configura el PIN como salida digital
 radio.begin();
  radio.openWritingPipe(identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  WiFi.begin(ssid, password);                           // Conecta a la red Wi-Fi especificada

  while (WiFi.status() != WL_CONNECTED) {                // Espera hasta que se establezca la conexión Wi-Fi
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  
  Serial.println("Conectado a Wi-Fi");
 // ledcAttachPin(pinZumbador, 0);                        // Asocia el pin del zumbador al canal 0 del generador de tonos LEDC
 
}

void loop() {
  
  int rssi = WiFi.RSSI();                                // Obtiene la intensidad de señal RSSI
  float distance = calculateDistance(rssi);              // Calcula la distancia basada en la intensidad de señal

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" metros");

  if (distance < 2.00) {
    
   char cadena[16];
   sprintf(cadena, "%d", i);
  
  
    enviar(cadena,"19.324.323-8","latitud");
    i++;
    beep();                                             // Emite un sonido si la distancia es menor a 2.00 metros
  } else {
   ledcWriteTone(0, 0);                                 // Detiene el zumbador si la distancia es mayor o igual a 2.00 metros
 }
  //PARTE DEL GPS
 // Parte del GPS
  while (GPSSerial.available() > 0) {
    gps.encode(GPSSerial.read());
    if (gps.location.isUpdated()) {
      double latitude = gps.location.lat();
      double longitude = gps.location.lng();
      
      Serial.print("Latitud: ");
      Serial.println(latitude, 6); // 6 decimales de precisión
      Serial.print("Longitud: ");
      Serial.println(longitude, 6); // 6 decimales de precisión
       
      // Parte del RF
      char text[50]; // Espacio para almacenar el mensaje (ajusta el tamaño según necesites)
      snprintf(text, sizeof(text), "Latitud: %.6f, Longitud: %.6f", latitude, longitude);
      radio.write(text, strlen(text)); // Envía el mensaje
    }
  }
 
  
  digitalWrite(PIN, rssi > REFERENCE_RSSI ? HIGH : LOW); // Controla el PIN según la intensidad de señal RSSI

  delay(1000);                                          // Espera 1 segundo antes de repetir el ciclo
}

float calculateDistance(int rssi) {
  // Fórmula de propagación de señal RSSI
  float exponent = (REFERENCE_RSSI - rssi) / 20.0;
  float distance = pow(10, exponent) * REFERENCE_DISTANCE;
  return distance;
}

void beep() {
  for (int i = 0; i < 10; ++i) {
    if (i % 2 == 0) {                                    // Genera un tono alternante
      ledcWriteTone(0, 2000);                            // Establece la frecuencia del tono en 2000 Hz
    } else {
      ledcWriteTone(0, 2500);                            // Establece la frecuencia del tono en 2500 Hz
   }
    delay(120);                                          // Espera 120 milisegundos antes de cambiar el tono
 }
}

bool enviar(char t1[],char t2[],char t3[]){//envia los datos al dispositivo central
  
  radio.write(t1, sizeof(t1));
  radio.write(t2, sizeof(t2));
   radio.write(t3, sizeof(t3));
  
   delay(1000); // Un segundo (1000 milisegundos) de espera entre envío y envío

  return true;
  }
