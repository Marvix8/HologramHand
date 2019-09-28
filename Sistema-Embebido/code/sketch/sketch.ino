/*
 * SerialOutput sketch
 * Print numbers to the serial port
*/
#include "lib/Kalman.h"

unsigned long interval = 4000; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

const int flexSensorMedio = A0;
const int flexSensorIndice = A1;
const int flexSensorPulgar = A2;

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
#define VCC = 4.98 // Measured voltage of Ardunio 5V line

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
#define STRAIGHT_RESISTANCE_FLEX_MEDIO  550.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_MEDIO      75.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_MEDIO   98.0 // Measured resistance of 3.3k resistor

// falta sacar medicion de este
#define STRAIGHT_RESISTANCE_FLEX_INDICE  550.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_INDICE      75.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_INDICE   98.0 // Measured resistance of 3.3k resistor

// falta sacar medicion de este
#define STRAIGHT_RESISTANCE_FLEX_PULGAR  550.0 // resistance when straight
#define BEND_RESISTANCE_FLEX_PULGAR      75.0 // resistance at 90 deg
#define DIVISOR_RESISTANCE_FLEX_PULGAR   98.0 // Measured resistance of 3.3k resistor

//Creates the variables:
double measurement, filteredMeasurement;
Kalman myFilter(0.125,32,1023,0); //suggested initial values for high noise filtering

void setup()
{
  pinMode(flexSensorMedio, INPUT);
  pinMode(flexSensorIndice, INPUT);
  pinMode(flexSensorPulgar, INPUT);
  Serial.begin(9600); // Initialize Serial communciation - send and receive at 9600 baud
}


void loop() 
{
  unsigned long currentMillis = millis(); // grab current time
 
  // Si interval es menor a la diferencia de currentMillis y previousMillis tomo las mediciones de los sensores.
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    float algo = readFlex(flexSensorMedio, STRAIGHT_RESISTANCE_FLEX_MEDIO, BEND_RESISTANCE_FLEX_MEDIO, DIVISOR_RESISTANCE_FLEX_MEDIO);
    float algo2 = readFlex(flexSensorPulgar, STRAIGHT_RESISTANCE_FLEX_PULGAR, BEND_RESISTANCE_FLEX_PULGAR, DIVISOR_RESISTANCE_FLEX_PULGAR);
    float algo3 = readFlex(flexSensorIndice, STRAIGHT_RESISTANCE_FLEX_INDICE, BEND_RESISTANCE_FLEX_INDICE, DIVISOR_RESISTANCE_FLEX_INDICE);

    
    // Actualizo previousMillis para que vuelva a ingresar en próximas iteraciones.
    previousMillis = millis();
 }

  //reads measurement and filter it

  /*
  // only for kalman
  filteredMeasurement = myFilter.getFilteredValue(measurement);
  Serial.println("KalmanFilter: " + String(filteredMeasurement) + " kalmanValue");
  Serial.println();
*/
}

float readFlex (const int sensorName, const int straightResistance, const int bendResistance, const int divisorResistance)
{
  int sensorValue = analogRead(sensorName); //read new value from sensor in analog pin A1, for example

  float flexVoltaje = sensorValue * VCC / 1023.0;
  float flexResistance = divisorResistance * (VCC / flexVoltaje - 1.0);

  // Uso la resistencia calculada para estimar el angulo de inclinación del sensor
  float angleValue = map(flexResistance, straightResistance, bendResistance, 0, 90.0);

  return angleValue;
}
