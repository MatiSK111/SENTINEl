/* Comunicacion inalambrica con NRF24L01 Arduino 
Ejemplo 1 – Programa para el Receptor
Libreria: 
TMRh20/RF24 https://github.com/tmrh20/RF24/ */

/* Debemos incluir la libreria SPI basica y 
la libreria RF24 que ya hemos instalado */
#include <SPI.h>
#include <RF24.h>

/* Crear un objeto RF24 al que llamaremos 
“radio”. Los dos argumentos que se utilizan 
en la funcion son los numeros de pin que 
vamos a usar para CS y CE. */
RF24 radio(9, 10); // CE, CSN

/* Creamos una matriz de bytes que represente la 
direccion con la que se reconoceran entre si los 
dos modulos. Podemos cambiar el valor de esta 
direccion a cualquier conjunto de 5 letras y/o 
numeros. Esta direccion permite elegir con que 
receptor vamos a comunicarnos. En nuestro caso, 
usaremos la misma direccion tanto en el receptor 
como en el transmisor. */
const byte identificacion[6] = "00001";

void setup() {
  Serial.begin(9600); // Iniciamos la comunicacion serie hacia el Monitor Serie en la PC

/* En la sección de configuracion debemos 
inicializar el objeto “radio” */
  radio.begin();

/* En el receptor establecemos la misma direccion 
que en el transmisor usando la funcion 
radio.setReadingPipe() y asi queda habilitada 
la comunicacion entre los dos modulos. */
  radio.openReadingPipe(0, identificacion);

/* Configuramos el nivel del amplificador de potencia,
usando la funcion radio.setPALevel(). Mientras 
estemos a este nivel de prueba, es decir prototipos 
sobre la mesa de trabajo, lo establecemos al minimo, 
ya que los modulos estaran cerca el uno del otro. 
Si se utiliza un nivel mas alto, se recomienda 
utilizar capacitores de filtro entre las entradas 
GND y 3,3 V de los mdulos para que su voltaje se 
mantenga mas estable durante el funcionamiento. */
  radio.setPALevel(RF24_PA_MIN);

/* La funcion radio.startListening() establece al 
modulo como receptor. */
  radio.startListening();
}

void loop() {
  if (radio.available()) {
  char texto1[64] = "67fytfhgfjhfchgcfjhf";
  char texto2[64] = "";
  char texto3[64] = "";


 
/* Creamos una matriz de caracteres donde recibir el 
mensaje */

  radio.read(&texto1, sizeof(texto1)); // Esperamos que llegue algo por RF desde el módulo
 // radio.read(&coordenadas, sizeof(coordenadas)); // Esperamos que llegue algo por RF desde el módulo
  radio.read(&texto2, sizeof(texto2)); // Esperamos que llegue algo por RF desde el módulo
  radio.read(&texto3, sizeof(texto3)); // Esperamos que llegue algo por RF desde el módulo

/* El símbolo “&” delante del nombre de la variable 
establece un indicador que señala a la variable que 
contiene los datos que queremos enviar y, con el 
segundo argumento, establecemos la cantidad de bytes 
que se van a usar de esa variable. En este caso, la 
funcion sizeof() nos devolvera la cantidad total de 
los bytes de la cadena "texto". */

  Serial.print(texto1);
  Serial.print(texto2);
  Serial.print(texto3);

  Serial.println();

   
/* Enviamos lo que hemos recibido al Monitor 
Serie en la PC */
 }
}
