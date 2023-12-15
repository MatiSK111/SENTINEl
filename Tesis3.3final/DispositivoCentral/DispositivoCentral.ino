#include <SPI.h>
#include <RF24.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

//const char* ssid ="Carvajal S";//credenciales wifi
//const char* password ="polo9191";

const char* ssid = "OnePlus10Pro5g";                          // Nombre de tu red Wi-Fi
const char* password = "niko9191";

RF24 radio(4, 5);  // Pines CE y CSN
const byte address[6] = "00001";  // Mismo canal que el transmisor

WiFiClient client; 

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.startListening();


 
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexión OK!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
  


  
}

void loop() {
  if (radio.available()) {
    char text[96] = "";  // Asegúrate de que el tamaño del buffer sea suficiente para contener los datos
    radio.read(&text, sizeof(text));
    String message = String(text);
    
    // Dividir los datos en las tres variables
    int separator1 = message.indexOf('|');
    int separator2 = message.indexOf('|', separator1 + 1);
    if (separator1 != -1 && separator2 != -1) {
      String data1 = message.substring(0, separator1);
      String data2 = message.substring(separator1 + 1, separator2);
      String data3 = message.substring(separator2 + 1);
      
      Serial.println("Dato 1: " + data1);
      Serial.println("Dato 2: " + data2);
      Serial.println("Dato 3: " + data3);

       EnvioDatos(data1, data2, data3);
    }
  }
}

void EnvioDatos(String data1, String data2, String data3){


  WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
 //   http.begin(client, "http://" + String(server) + ":" + String(port) + path);
  
  if (WiFi.status() == WL_CONNECTED){ 
    // HTTPClientSecure http;  // creo el objeto http
     String datos_a_enviar = "rut=" + String(data1) + "&latitud=" + String(data2)+ "&longitud=" + String(data3);  

     http.begin(client,"https://www.gembersoft.com/sentinel/Microservicios/EspPost.php");
     http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // defino texto plano..

     int codigo_respuesta = http.POST(datos_a_enviar);

     

     if (codigo_respuesta>0){
      Serial.println("Código HTTP: "+ String(codigo_respuesta));
        if (codigo_respuesta == 200){
          String cuerpo_respuesta = http.getString();
          Serial.println("El servidor respondió: ");
          Serial.println(cuerpo_respuesta);
         mensajeLed(2);

        }
     } else {
        Serial.print("Error enviado POST, código: ");
        Serial.println(codigo_respuesta);

        Serial.print("Error enviado POST: ");
      Serial.println(http.errorToString(codigo_respuesta).c_str());
        mensajeLed(3);
     }

       http.end();  // libero recursos
       
  } else {
     Serial.println("Error en la conexion WIFI");
       mensajeLed(4);
  }
 // delay(30000); //espera 60s
}


void mensajeLed(int cant) {
int i = cant;

 while(i>=1){ 
    
     dacWrite(25,250);//led al maximo
     delay(300);
     dacWrite(25,0);
     delay(300);
   
    i=i-1;
    }

  
}
