#include "MPU9250.h"
#include "Kalman.h"
// variables bias de calibración
float axb;
float ayb;
float azb;

// variables factor de calibración
float axs;
float ays;
float azs;

// variables de lectura una vez calibrad
float ax;
float ay;
float az;

double filteredMeasurementX;
double filteredMeasurementY;
double filteredMeasurementZ;

MPU9250 IMU(Wire,0x68);
Kalman acelerometerXK(0.7, 15, 1023, 0);
Kalman acelerometerYK(0.8, 15, 1023, 0);
Kalman acelerometerZK(0.8, 8, 1023, 0);

int status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.write("begin");
  status = IMU.begin();
  status = IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  status = IMU.calibrateAccel();
  
  // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
  
  //Serial.print("X\tY\tZ");
  Serial.print("A\tXK\tYK\tZK\tB");
  Serial.println("");

  /* El siguiente bloque de código se debería utilizar para calibrar con la salida del programa 

  float axb = 0.01; // accel bias of 0.01 m/s/s
  float axs = 1.0; // accel scale factor of 0.97
  IMU.setAccelCalX(axb,axs);

  float ayb = 0.01; // accel bias of 0.01 m/s/s
  float ays = 1.0; // accel scale factor of 0.97
  IMU.setAccelCalY(ayb,ays);

  float azb = 0.01; // accel bias of 0.01 m/s/s
  float azs = 1.0; // accel scale factor of 0.97
  IMU.setAccelCalZ(azb,azs);*/

  

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print(0.8, DEC);
  Serial.print("\t");
  status = IMU.readSensor();
  ax = IMU.getAccelX_mss() * 0.1;
  ay = IMU.getAccelY_mss() * 0.1;
  az = IMU.getAccelZ_mss() * 0.1;

  filteredMeasurementX = acelerometerXK.getFilteredValue(ax);
  filteredMeasurementY = acelerometerYK.getFilteredValue(ay);
  filteredMeasurementZ = acelerometerZK.getFilteredValue(az);
  
  Serial.print(filteredMeasurementX);
  Serial.print("\t");
  Serial.print(filteredMeasurementY);
  Serial.print("\t");
  Serial.print(filteredMeasurementZ);
  Serial.print("\t");
  Serial.print(-0.8, DEC);
  
  Serial.println(""); 
  delay(100);
}
