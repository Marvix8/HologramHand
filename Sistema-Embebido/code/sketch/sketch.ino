/*
 * SerialOutput sketch
 * Print numbers to the serial port
*/

const int flexSensorMedio = A0;

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 98.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 550.0; // resistance when straight
const float BEND_RESISTANCE = 75.0; // resistance at 90 deg

void setup()
{
  pinMode(flexSensorMedio, INPUT);
  Serial.begin(9600); // Initialize Serial communciation - send and receive at 9600 baud
}


void loop()
{
  // Read the ADC, and calculate voltage and resistance from it
  int flexADC = analogRead(flexSensorMedio);
  unsigned int kalmanValue;
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  Serial.println("Resistance: " + String(flexR) + " ohms");

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
    0, 90.0);
  Serial.println("Bend: " + String(angle) + " degrees");

  kalmanValue = Kalman_filter_for_SEN_1(flexADC);
  Serial.println("KalmanValue: " + String(kalmanValue) + " kalman filter");
  Serial.println();

  delay(4000);
}


unsigned int Kalman_filter_for_SEN_1(signed int ADC_Value)
{
  static unsigned char counter = 0;
  static float A,H,Q,R,X,P;
  float XP,PP;
  static float value_return=0;
  float K;
  float temp_float;
 
  if(counter < 1)
     counter++;
 
  if(counter == 1)
  {
    A = 1;
    H = 1;   //1
    Q = 0.22;     //0.32 earlier 0.92,0.02
    R = 0.9;     // 0.8
    X = 1023;
    P = 6;
    counter = 2;
 
    XP = A*X;
    PP = A*P*A + Q;
 
    K = PP * H;
    K /= (H * H * PP) + R;
 
    temp_float = (float)(ADC_Value - H * XP);
    value_return = XP + K * temp_float;
 
    temp_float = H*PP;
    P = PP-K*temp_float;
    return (unsigned int)value_return;
  }
  else
  {
    XP = A*value_return;
    PP = A*P*A + Q;
 
    K = PP*H;
    K /=(H*H*PP)+R;
 
    temp_float = (float)(ADC_Value-H*XP);
    value_return = XP + K*temp_float;
 
    temp_float = (float)H*PP;
    P = PP-K*temp_float;
    return (unsigned int)value_return;
  }
}
