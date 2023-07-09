#include <WiFi.h>
#include <math.h>

const int PIN = 2;                                      // Pin utilizado para controlar una salida digital
const char* ssid = "SENTINEL";                          // Nombre de tu red Wi-Fi
const char* password = "12345678";                      // Contraseña de tu red Wi-Fi
const int REFERENCE_RSSI = -50;                          // Intensidad de señal RSSI a una distancia conocida
const float REFERENCE_DISTANCE = 1.0;                    // Distancia conocida en metros para la intensidad de señal RSSI de referencia
const int pinZumbador = 4;                               // Pin utilizado para controlar el zumbador

void setup() {
  Serial.begin(115200);                                 // Inicia la comunicación serie a una velocidad de 115200 bps
  pinMode(PIN, OUTPUT);                                 // Configura el PIN como salida digital

  WiFi.begin(ssid, password);                           // Conecta a la red Wi-Fi especificada

  while (WiFi.status() != WL_CONNECTED) {                // Espera hasta que se establezca la conexión Wi-Fi
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }

  Serial.println("Conectado a Wi-Fi");
  ledcAttachPin(pinZumbador, 0);                        // Asocia el pin del zumbador al canal 0 del generador de tonos LEDC
}

void loop() {
  int rssi = WiFi.RSSI();                                // Obtiene la intensidad de señal RSSI
  float distance = calculateDistance(rssi);              // Calcula la distancia basada en la intensidad de señal

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" metros");

  if (distance < 2.00) {
    beep();                                             // Emite un sonido si la distancia es menor a 2.00 metros
  } else {
    ledcWriteTone(0, 0);                                 // Detiene el zumbador si la distancia es mayor o igual a 2.00 metros
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
