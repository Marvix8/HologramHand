/*
   SerialOutput sketch
   Print numbers to the serial port
*/
#include "lib/Flex.h"
#include "lib/Hand.h"
#include "lib/Gesture.h"
#include "lib/MPU9250.h"
#include "lib/Kalman.h"
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
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Declarar objetos de sensores
Flex bigFinger((double)DIVISOR_RESISTANCE_FLEX_PULGAR);
Flex indexFinger((double)DIVISOR_RESISTANCE_FLEX_INDICE);
Flex middleFinger((double)DIVISOR_RESISTANCE_FLEX_MEDIO);
// Objeto de acelerómetros
MPU9250 IMU(Wire,0x68);
Acelerometer acelerometer;

Hand hand(bigFinger, indexFinger, middleFinger, acelerometer);
Gesture gesture(hand);

// variables para verificar posición de mano
int previousPosition = -1;
int actualPosition = -1;
int counterSamePosition = 0;

// auxiliares para lectura de sensor flex
double fatFingerFlex;
double indexFingerFlex;
double middleFingerFlex;

// auxiliares para kalman
double filteredMeasurementX;
double filteredMeasurementY;
double filteredMeasurementZ;

// Kalman para acelerometro
Kalman acelerometerXK(0.7, 15, 1023, 0);
Kalman acelerometerYK(0.8, 15, 1023, 0);
Kalman acelerometerZK(0.8, 8, 1023, 0);

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
  /*pinMode(BLUE_LED, OUTPUT);
  pinMode(RGB_LED_RED, OUTPUT);
  pinMode(RGB_LED_BLUE, OUTPUT);
  pinMode(RGB_LED_GREEN, OUTPUT);*/

  //bluetooth.begin(9600); // Inicializa comunicación bluetooth.
  Serial.begin(9600); // Inicializa comunicación en serie.

  Wire.write("begin");
  IMU.begin();
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
}

/*
   Sentencias a ejecutar constantemente en el arduino.
*/
void loop()
{
  if(bluetooth.available()){

    if(bluetooth.read() == "A") {
      hand.calibrateStraightHand((double)analogRead(FLEX_SENSOR_PULGAR), (double)analogRead(FLEX_SENSOR_INDICE), (double)analogRead(FLEX_SENSOR_MEDIO));  
    }

    if(bluetooth.read() == "B") {
      hand.calibrateBendHand((double)analogRead(FLEX_SENSOR_PULGAR), (double)analogRead(FLEX_SENSOR_INDICE), (double)analogRead(FLEX_SENSOR_MEDIO));
      hand.setCalibrated();
    }
    
    if(hand.getCalibrated() == true) {
      fatFingerFlex = (double)analogRead(FLEX_SENSOR_PULGAR);
      indexFingerFlex = (double)analogRead(FLEX_SENSOR_INDICE);
      middleFingerFlex = (double)analogRead(FLEX_SENSOR_MEDIO);

      hand.processPosition(fatFingerFlex, indexFingerFlex, middleFingerFlex);
      
      if (previousPosition == -1){
        previousPosition = hand.getHandPosition();
      } else {
        actualPosition = hand.getHandPosition();
      }
      
      if (actualPosition == previousPosition ){
        counterSamePosition++;
      } else {
        actualPosition = hand.getHandPosition();
        previousPosition = actualPosition;
        counterSamePosition = 0;
      }
      
      IMU.readSensor();
      filteredMeasurementX = acelerometerXK.getFilteredValue(IMU.getAccelX_mss() * 0.1);
      filteredMeasurementY = acelerometerYK.getFilteredValue(IMU.getAccelY_mss() * 0.1);
      filteredMeasurementZ = acelerometerZK.getFilteredValue(IMU.getAccelZ_mss() * 0.1);
      
      if(counterSamePosition == 20) {
        acelerometer.setAxisValues(filteredMeasurementX, filteredMeasurementY, filteredMeasurementZ);
        
        hand.process();
        gesture.readInput();
        gesture.updateStateMachine();

        if(gesture.getAction() != -1 && gesture.getHasChanged() == true) {
          bluetooth.write(gesture.getAction());
        }       
        
        counterSamePosition = 0;
      }
    }
  }
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
