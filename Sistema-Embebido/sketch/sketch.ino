/*
   SerialOutput sketch
   Print numbers to the serial port
*/
#include "lib/Flex.h"
#include "lib/Hand.h"
//#include "lib/Gesture.h"
#include <SoftwareSerial.h>    // para la comunicación bluetooth

/*
   Utilizadas para marcar el intervalo de lectura de medición de los sensores.
*/
unsigned long interval = 4000; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

/*
   Asignación de sensores a los distintos pines del arduino.
*/
#define FLEX_SENSOR_PULGAR  A2
#define FLEX_SENSOR_INDICE  A1
#define FLEX_SENSOR_MEDIO   A0
#define BLUETOOTH_RX        0
#define BLUETOOTH_TX        1
#define BLUE_LED            2
#define RGB_LED_RED         3
#define RGB_LED_BLUE        4
#define RGB_LED_GREEN       5

/*
   Voltaje que reciven los sensores
*/
#define VCC 4.98 // Measured voltage of Ardunio 5V line

/*
   Valor de las resistencias de cada sensor flex.
   @BEND_RESISTANCE es la medición con el flex cerrado.
   @STRAIGHT_RESISTANCE es la medicón con el flex completmente abierto.
   @DIVISOR_RESISTANCE es la medición de la resistencia fija del divisor de tensión.
*/

// falta sacar medicion de este
#define STRAIGHT_RESISTANCE_FLEX_PULGAR  750.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_PULGAR      662.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_PULGAR   98.0 // Measured resistance of 3.3k resistor

// falta sacar medicion de este
#define STRAIGHT_RESISTANCE_FLEX_INDICE  750.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_INDICE      520.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_INDICE   98.0 // Measured resistance of 3.3k resistor

#define STRAIGHT_RESISTANCE_FLEX_MEDIO  775.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_MEDIO      504.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_MEDIO   98.0 // Measured resistance of 3.3k resistor

// Valor a enviar vía bluetooth.
int mensajeBluetooth;

// Declarar objeto de comunicación bluetooth.
//SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Declarar objetos de sensores
Flex bigFinger((double)DIVISOR_RESISTANCE_FLEX_PULGAR);
Flex indexFinger((double)DIVISOR_RESISTANCE_FLEX_INDICE);
Flex middleFinger((double)DIVISOR_RESISTANCE_FLEX_MEDIO);
Hand hand(bigFinger, indexFinger, middleFinger);
//Gesture gesture(hand);

// Prototipos de funciones
void sendInformationViaBluetooth (const int);
void rgbColor (int, int, int);


/*
   Configuración inicial de pines y comunicación.
*/
void setup()
{
  pinMode(FLEX_SENSOR_PULGAR, INPUT);
  pinMode(FLEX_SENSOR_INDICE, INPUT);
  pinMode(FLEX_SENSOR_MEDIO, INPUT);
  //pinMode(BLUE_LED, OUTPUT);
  /*pinMode(RGB_LED_RED, OUTPUT);
  pinMode(RGB_LED_BLUE, OUTPUT);
  pinMode(RGB_LED_GREEN, OUTPUT);*/

  //bluetooth.begin(9600); // Inicializa comunicación bluetooth.
  Serial.begin(9600); // Inicializa comunicación en serie.
}

/*
   Sentencias a ejecutar constantemente en el arduino.
*/
void loop()
{
  // Obtener tiempo actual
  unsigned long currentMillis = millis();

  if (hand.getCalibrated() == false){
      // calibro dedos
      hand.calibrateStraightHand((double)STRAIGHT_RESISTANCE_FLEX_PULGAR, (double)STRAIGHT_RESISTANCE_FLEX_INDICE,(double)STRAIGHT_RESISTANCE_FLEX_MEDIO); 
      hand.calibrateStraightHand((double)BEND_RESISTANCE_FLEX_PULGAR, (double)BEND_RESISTANCE_FLEX_INDICE,(double)BEND_RESISTANCE_FLEX_MEDIO); 
      hand.setCalibrated();;
  }

  // Si interval es menor a la diferencia de currentMillis y previousMillis tomo las mediciones de los sensores.
  if ((unsigned long)(currentMillis - previousMillis) >= interval)
  {
    /*if(bluetooth.available())
    {
      digitalWrite(BLUE_LED, HIGH);*/
      // llamar a hand
      hand.process((double)analogRead(FLEX_SENSOR_PULGAR), (double)analogRead(FLEX_SENSOR_INDICE), (double)analogRead(FLEX_SENSOR_MEDIO));

      //int test = hand.getHandPosition();

      Serial.println("P");
      Serial.println((double)analogRead(FLEX_SENSOR_PULGAR));
      //Serial.println(bigFinger.processInformation((double)analogRead(FLEX_SENSOR_PULGAR)));
      Serial.println(hand.getBigFinger().getActualValue());
      Serial.println(bigFinger.getActualValue());
      /*Serial.println(hand.getAux());
      Serial.println(bigFinger.getActualValue());
      Serial.println(bigFinger.getAngle());*/
      
      Serial.println("\n");
          
/*
      Serial.println("I");
      Serial.println((double)analogRead(FLEX_SENSOR_INDICE));
      Serial.println(indexFinger.getActualValue());
      Serial.println(indexFinger.getAngle());
      Serial.println("\n");

      Serial.println("M");
      Serial.println((double)analogRead(FLEX_SENSOR_MEDIO));
      Serial.println(middleFinger.getActualValue());
      Serial.println(middleFinger.getAngle());
      Serial.println("\n");*/
      /*
      Serial.println("EVALUO");
      if (test == 0){
        Serial.println("STAR");
      } else if (test == 1) {
        Serial.println("SCISSORS");
      } else if (test == 2) {
        Serial.println("GOOD");
      } else if (test == 3) {
        Serial.println("ROCK");
      }*/
      
      
      // llamar a gesture --> si tuviera unooooooo!!!
  
      // Actualizo previousMillis para que vuelva a ingresar en próximas iteraciones.
    
      previousMillis = millis();
  }

  /*
    // Ejemplo de invocación a Kalman.. nos va a servir para el acelerómetro.
    filteredMeasurement = myFilter.getFilteredValue(measurement);
    Serial.println("KalmanFilter: " + String(filteredMeasurement) + " kalmanValue");
    Serial.println();
  */
}


/*
 * Función utilizada para enviar información vía Bluetooth
 * 
 * Recibe:
 * @movement: movimiento a enviar a la aplicación Android.
 */
 /*
void sendInformationViaBluetooth (const int movement)
{
  bluetooth.write(movement);
}*/


/*
 * Función utilizada para cambiar el color de led RGB
 * Entrada:
 * @redLightValue: cantidad de color rojo.
 * @blueLightValue: cantidad de color azul.
 * @greenLightValue: cantidad de color verde.
 */
 /*
void rgbColor (int redLightValue, int greenLightValue, int blueLightValue)
{
  analogWrite(RGB_LED_RED, redLightValue);
  analogWrite(RGB_LED_BLUE, blueLightValue);
  analogWrite(RGB_LED_GREEN, greenLightValue);
}*/
