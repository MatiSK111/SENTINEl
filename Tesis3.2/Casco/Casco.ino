#include <WiFi.h>
#include <math.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN
const byte address[6] = "00001";  // Dirección del canal
#define RXPin 16  // Pin RX del GPS conectado al pin 16 del ESP32
#define TXPin 17  // Pin TX del GPS conectado al pin 17 del ESP32
#define LED_PIN 12
const int buzzer = 2;                                      // Pin utilizado para controlar una salida digital
const char* ssid = "SENTINEL";                          // Nombre de tu red Wi-Fi
const char* password = "12345678";  // Contraseña de la red Wi-Fi
//const char* ssid = "iPhone de Gerardo";                          // Nombre de tu red Wi-Fi
//const char* password = "niko9191";

const int REFERENCE_RSSI = -50;                          // Intensidad de señal RSSI a una distancia conocida
const float REFERENCE_DISTANCE = 1.0;  // Distancia conocida en metros para la intensidad de señal RSSI de referencia

int canal=0, frec=4000, resolucion=8;
int amplitud = 255; 
/// parte del contador temporal 

unsigned long tiempoInicial = 0;
unsigned long tiempoActual = 0;
const unsigned long tiempoMaximo = 180000; // 3 minutos en milisegundos



HardwareSerial GPSSerial(1);  // Utiliza Serial1 para comunicarse con el módulo GPS
TinyGPSPlus gps;
int i=0;
String lo="1";
String la="1";
bool haylocacion =false;

void setup() {
  
  Serial.begin(9600);                                 // Inicia la comunicación serie a una velocidad de 115200 bps
  GPSSerial.begin(9600, SERIAL_8N1, RXPin, TXPin);

  ledcSetup(canal,frec,resolucion);
  ledcAttachPin(buzzer,canal);
    ledcWrite(canal, amplitud); 
  //pinMode(buzzer, OUTPUT); 
  // Configura el PIN como salida digital
 // pinMode(LED_PIN, OUTPUT);
 radio.begin();
   radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);  // Potencia de transmisión alta
  ledcWriteTone(0,0);
  WiFi.begin(ssid, password);                           // Conecta a la red Wi-Fi especificada
  //noTone(buzzer);
  while (WiFi.status() != WL_CONNECTED) {                // Espera hasta que se establezca la conexión Wi-Fi
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  
  Serial.println("Conectado a Wi-Fi");
 // ledcAttachPin(pinZumbador, 0);                        // Asocia el pin del zumbador al canal 0 del generador de tonos LEDC



}

void loop() {
  
  int rssi = WiFi.RSSI();                                // Obtiene la intensidad de señal RSSI
  float distance = calculateDistance(rssi); // Calcula la distancia basada en la intensidad de señal

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" metros");
  obtenergps();
  //noTone(buzzer);
  if (distance < 2.00 && distance>0.01) {
    
   char cadena[16];
   sprintf(cadena, "%d", i);
  
    String data1 = "1";
   // enviar(data1, data2, data3);
    i++;
    enviar(data1, la, lo);
    beep();// Emite un sonido si la distancia es menor a 2.00 metros
     delay(3000);
  } else {
   ledcWriteTone(0, 0);                                 // Detiene el zumbador si la distancia es mayor o igual a 2.00 metros
 }

 // Parte del GPS
 /* while (GPSSerial.available() > 0 ) {
    gps.encode(GPSSerial.read());
    if (gps.location.isUpdated()) {
      double latitude = gps.location.lat();
      double longitude = gps.location.lng();
      haylocacion=true;
       Serial.println("hay locacion");
      Serial.print("Latitud: ");
      Serial.println(latitude, 6); // 6 decimales de precisión
      Serial.print("Longitud: ");
      Serial.println(longitude, 6); // 6 decimales de precisión

        la = String(latitude,6);//queda en string listo para enviar
        lo= String(longitude,6);//queda en string listo para enviar
      
    }else{
      haylocacion=false;
     Serial.println("no hay locacion");
        //la = "00";
       // lo= "00";
      }
  }
 */
  
 //a digitalWrite(buzzer, rssi > REFERENCE_RSSI ? HIGH : LOW); // ojito aqui

  delay(500);                     // Espera 1 segundo antes de repetir el ciclo
}

void obtenerlocacion(String rut){

  double latdef = 1;//latitud por defecto
  double londef = 1;//longitud por defecto

  if(GPSSerial.available() > 0){
    gps.encode(GPSSerial.read());
      if(gps.location.isUpdated()) {
        
        double latdef = gps.location.lat();//cambia las coordenadas si hay
        double londef = gps.location.lng();
                     Serial.print("mando gps weno");

      }else{

        String latitud="";//convertir de double a string
        latitud = String(latdef,6);//queda en string listo para enviar
        
        String longitud="";//convertir de double a string
        longitud = String(londef,6);//queda en string listo para enviar
        
         enviar(rut, latitud, longitud);
              Serial.print("Latitud: ");
              Serial.println(latitud); 
              Serial.print("Longitud: ");
              Serial.println(longitud);
        
        }
   }else{
    
      String latitud="";//convertir de double a string
  latitud = String(latdef,6);//queda en string listo para enviar
  
  String longitud="";//convertir de double a string
  longitud = String(londef,6);//queda en string listo para enviar
  
  
   enviar(rut, latitud, longitud);
        Serial.print("Latitud: ");
        Serial.println(latitud); 
        Serial.print("Longitud: ");
        Serial.println(longitud);
 
    
    }

}
void obtenergps(){

  if(GPSSerial.available() > 0){
    gps.encode(GPSSerial.read());
      if(gps.location.isUpdated()) {
        
        double latdef = gps.location.lat();//cambia las coordenadas si hay
        double londef = gps.location.lng();
        Serial.print("mando gps weno...............................");
         la = String(latdef,6);//queda en string listo para enviar
        lo= String(londef,6);//queda en string listo para enviar
        tiempoInicial = millis(); // Inicia el temporizador cuando se actualiza
         Serial.print("Inicio contador");
        haylocacion=true;
      }else{
            tiempoActual = millis(); // Actualiza el tiempo actual
            if (tiempoActual - tiempoInicial >= tiempoMaximo) {
             Serial.println("contador en 0 ya no hay locacion"); 
            haylocacion = false; // Cambia haylocacion a falso
            la ="0";
            lo="0";
            }
        Serial.println("no hay locacion");
        }
   }

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
      //ledcWriteTone(0, 3000);    
      //digitalWrite(LED_PIN, HIGH);  // Encender el LED conectado al pin D12
      dacWrite(25,250);//led al maximo
      dacWrite(26,250);//beeper al maximo (255)

      
      // Establece la frecuencia del tono en 2000 Hz
      
    //tone (buzzer,3000);
    } else {
     // ledcWriteTone(0,0);
        //digitalWrite(LED_PIN, LOW);  // Apagar el LED conectado al pin D12
        dacWrite(25,0);
        dacWrite(26,0);
      // Establece la frecuencia del tono en 2500 Hz
     // noTone(buzzer);
   }
    delay(300);                                          // Espera 120 milisegundos antes de cambiar el tono
 }
}

void enviar(String data1, String data2, String data3) {
  
  String message = data1 + "|" + data2 + "|" + data3;  // Concatenamos los datos con un separador
  radio.write(message.c_str(), message.length());
  Serial.println("Datos enviados: " + message);
  
}
