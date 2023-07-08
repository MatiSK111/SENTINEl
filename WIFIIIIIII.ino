#include <WiFi.h>
#include <math.h>

const int PIN = 2;
const char* ssid = "iPhone de matia";      // Nombre de tu red Wi-Fi
const char* password = "12345678";     // Contraseña de tu red Wi-Fi
const int REFERENCE_RSSI = -50;          // Intensidad de señal RSSI a una distancia conocida
const float REFERENCE_DISTANCE = 1.0;    // Distancia conocida en metros para la intensidad de señal RSSI de referencia
const int pinZumbador=4;

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }

  Serial.println("Conectado a Wi-Fi");
  ledcAttachPin(pinZumbador, 0);
}

void loop() {
  int rssi = WiFi.RSSI();
  float distance = calculateDistance(rssi);

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" metros");
  if(distance < 2.00){
    beep();
  }else{
    ledcWriteTone(0, 0); 
  }

  digitalWrite(PIN, rssi > REFERENCE_RSSI ? HIGH : LOW);

  delay(1000);
}

float calculateDistance(int rssi) {
  // Fórmula de propagación de señal RSSI
  float exponent = (REFERENCE_RSSI - rssi) / 20.0;
  float distance = pow(10, exponent) * REFERENCE_DISTANCE;
  return distance;
}
void beep(){
  
      
      for (int i = 0; i< 10; ++i) {
        //Enviamos un tono 
    
        if (i % 2 == 0){ // La condición que hemos impuesto
          ledcWriteTone(0, 2000); 
        }
        else {
             ledcWriteTone(0, 2500); 
        }
        //La frecuencia incrementara de 30 en 30
       
        //Esperamos 1 segundo
        delay(120);
      }
      
  }
