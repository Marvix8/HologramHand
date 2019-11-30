#include "lib/Flex.h"
#include "lib/Hand.h"
#include "lib/Gesture.h"
#include "lib/MPU9250.h"
#include "lib/Kalman.h"
#include <SoftwareSerial.h>    // para la comunicación bluetooth

/*
   Asignación de sensores a los distintos pines del arduino.
*/
#define FLEX_SENSOR_PULGAR  A1
#define FLEX_SENSOR_INDICE  A0
#define FLEX_SENSOR_MEDIO   A2
#define BLUETOOTH_RX        7
#define BLUETOOTH_TX        8
#define BLUE_LED            4
#define RGB_LED_RED         9
#define RGB_LED_BLUE        5
#define RGB_LED_GREEN       6

/*
   Voltaje que reciven los sensores
*/
#define VCC 4.98 // Measured voltage of Ardunio 5V line

/*
   Inputs recibidos desde bluetooth
*/
#define CALIBRATE_STRAIGHT_HAND 83  // S
#define CALIBRATE_BEND_HAND 66      // B
#define TURN_LED 76                 // L

// Valor de la resistencia del divisor de tensión de cada flex.
#define DIVISOR_RESISTANCE_FLEX 100

// Declarar objeto de comunicación bluetooth.
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Declarar objetos de sensores
Flex bigFinger((double)DIVISOR_RESISTANCE_FLEX, 0.5);
Flex indexFinger((double)DIVISOR_RESISTANCE_FLEX, 0.8);
Flex middleFinger((double)DIVISOR_RESISTANCE_FLEX, 0.5);

// Objeto de acelerómetros
MPU9250 IMU(Wire, 0x68);
Acelerometer acelerometer;

Hand hand(bigFinger, indexFinger, middleFinger, acelerometer);
Gesture gesture(hand);

// variables para verificar posición de mano
int previousPosition = -1;
int actualPosition = -1;
int counterSamePosition = 0;

// auxiliares para lectura de sensor flex
double bigFingerFlex;
double indexFingerFlex;
double middleFingerFlex;

// auxiliares para kalman
double filteredMeasurementX;
double filteredMeasurementY;
double filteredMeasurementZ;

// lectura de bluetooth
int bluetoothReader;
char bluetoothSender;

// Kalman para acelerometro
Kalman acelerometerXK(0.7, 15, 1023, 0);
Kalman acelerometerYK(0.8, 15, 1023, 0);
Kalman acelerometerZK(0.8, 8, 1023, 0);

// Prototipos de funciones
void sendInformationViaBluetooth (const int);
void rgbColor (int, int, int);
void ledRGB();

/*
   Configuración inicial de pines y comunicación.
*/
void setup()
{
  pinMode(FLEX_SENSOR_PULGAR, INPUT);
  pinMode(FLEX_SENSOR_INDICE, INPUT);
  pinMode(FLEX_SENSOR_MEDIO, INPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RGB_LED_RED, OUTPUT);
  pinMode(RGB_LED_BLUE, OUTPUT);
  pinMode(RGB_LED_GREEN, OUTPUT);


  Serial.begin(9600); // Inicializa comunicación en serie.
  bluetooth.begin(9600); // Inicializa comunicación bluetooth.

  IMU.begin();
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
}

/*
   Sentencias a ejecutar constantemente en el arduino.
*/
void loop()
{
  if (bluetooth.available()) {
    bluetoothReader = bluetooth.read();

    if (bluetoothReader == CALIBRATE_BEND_HAND) {
      hand.calibrateBendHand((double)analogRead(FLEX_SENSOR_PULGAR), (double)analogRead(FLEX_SENSOR_INDICE), (double)analogRead(FLEX_SENSOR_MEDIO));
    }

    if (bluetoothReader == CALIBRATE_STRAIGHT_HAND) {
      hand.calibrateStraightHand((double)analogRead(FLEX_SENSOR_PULGAR), (double)analogRead(FLEX_SENSOR_INDICE), (double)analogRead(FLEX_SENSOR_MEDIO));
      hand.setCalibrated(true);
    }

    if (bluetoothReader == TURN_LED) {
      if (digitalRead(BLUE_LED) == LOW) {
        digitalWrite(BLUE_LED , HIGH);
      } else {
        digitalWrite(BLUE_LED , LOW);
      }
    }
  }

  if (hand.getCalibrated() == true) {
    bigFingerFlex = (double)analogRead(FLEX_SENSOR_PULGAR);
    indexFingerFlex = (double)analogRead(FLEX_SENSOR_INDICE);
    middleFingerFlex = (double)analogRead(FLEX_SENSOR_MEDIO);

    hand.processPosition(bigFingerFlex, indexFingerFlex, middleFingerFlex);

    if (previousPosition == -1) {
      previousPosition = hand.getHandPosition();
    }
    actualPosition = hand.getHandPosition();

    if (actualPosition == previousPosition) {
      counterSamePosition++;
    } else {
      counterSamePosition = 0;
    }
    previousPosition = actualPosition;

    IMU.readSensor();
    filteredMeasurementX = acelerometerXK.getFilteredValue(IMU.getAccelX_mss() * 0.1);
    filteredMeasurementY = acelerometerYK.getFilteredValue(IMU.getAccelY_mss() * (-0.1));
    filteredMeasurementZ = acelerometerZK.getFilteredValue(IMU.getAccelZ_mss() * 0.1);

    if (counterSamePosition == 1500) {
      acelerometer.setAxisValues(filteredMeasurementX, filteredMeasurementY, filteredMeasurementZ);

      hand.process();
      gesture.readInput();
      gesture.updateStateMachine();

      ledRGB();

      if (gesture.getAction() != '9') {
        bluetoothSender = gesture.getAction();
        bluetooth.write(bluetoothSender);
      }
      counterSamePosition = 0;
    }
  }
  else {
    rgbColor(LOW, LOW, LOW);
  }
}

/*
   Función utilizada para seleccionar el color a encender
   del led RGB
*/
void ledRGB() {
  switch (hand.getMovement()) {
    case (int)SPZ: rgbColor(255, 0, 0); break; // rojo
    case (int)SNZ: rgbColor(0, 255, 0); break; // verde
    case (int)SPY: rgbColor(0, 0, 255); break; // azul
    case (int)SPX: rgbColor(255, 255, 255); break; // blanco
    case (int)RPY: rgbColor(255, 255, 0); break; // amarillo
    case (int)RPZ: rgbColor(255, 0, 255); break; // magenta
    case (int)GNZ: rgbColor(0, 255, 255); break; // cyan
    case (int)GPZ: rgbColor(0, 255, 148); break; // violeta
    case (int)GPX: rgbColor(60, 40, 0); break; // naranja
    case (int)SCPY: rgbColor(128, 0, 0); break; // marrón
    case (int)SCNZ: rgbColor(0, 0, 128); break; // navy
    case (int)UNDEFINED: rgbColor(LOW, LOW, LOW); break; // apagado
  }
}


/*
   Función utilizada para cambiar el color de led RGB
   Entrada:
   @redLightValue: cantidad de color rojo.
   @blueLightValue: cantidad de color azul.
   @greenLightValue: cantidad de color verde.
*/
void rgbColor (int redLightValue, int greenLightValue, int blueLightValue)
{
  analogWrite(RGB_LED_RED, redLightValue);
  analogWrite(RGB_LED_BLUE, blueLightValue);
  analogWrite(RGB_LED_GREEN, greenLightValue);
}
