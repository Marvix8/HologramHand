/*
MPU9250.cpp
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Arduino.h"
#include "lib/MPU9250.h"

/* MPU9250 object, input the I2C bus and address */
MPU9250::MPU9250(TwoWire &bus,uint8_t address){
  _i2c = &bus; // I2C bus
  _address = address; // I2C address
}

/* starts communication with the MPU-9250 */
int MPU9250::begin(){
  // using I2C for communication
  // starting the I2C bus
  _i2c->begin();
  // setting the I2C clock
  _i2c->setClock(_i2cRate);

  // enable I2C master mode
  if(writeRegister(USER_CTRL,I2C_MST_EN) < 0){
    return -2;
  }
  // set the I2C bus speed to 400 kHz
  if(writeRegister(I2C_MST_CTRL,I2C_MST_CLK) < 0){
    return -3;
  }
  // set AK8963 to Power Down
  writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN);

  // wait for MPU-9250 to come back up
  delay(1);
  // reset the AK8963
  writeAK8963Register(AK8963_CNTL2,AK8963_RESET);

  // check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
  if((whoAmI() != 113)&&(whoAmI() != 115)){
    return -5;
  }
  // enable accelerometer and gyro
  if(writeRegister(PWR_MGMNT_2,SEN_ENABLE) < 0){
    return -6;
  }
  // setting accel range to 8G as default
  if(writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_8G) < 0){
    return -7;
  }
  _accelScale = G * 8.0f/32767.5f; // setting the accel scale to 8G
  _accelRange = ACCEL_RANGE_8G;

  // setting the sample rate divider to 0 as default
  if(writeRegister(SMPDIV,0x00) < 0){ 
    return -11;
  } 

  // enable I2C master mode
  if(writeRegister(USER_CTRL,I2C_MST_EN) < 0){
  	return -12;
  }
	// set the I2C bus speed to 400 kHz
	if( writeRegister(I2C_MST_CTRL,I2C_MST_CLK) < 0){
		return -13;
	}
	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if( whoAmIAK8963() != 72 ){
    return -14;
	}
  /* get the magnetometer calibration */
  // set AK8963 to Power Down
  if(writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) < 0){
    return -15;
  }
  delay(100); // long wait between AK8963 mode changes
  // set AK8963 to FUSE ROM access
  if(writeAK8963Register(AK8963_CNTL1,AK8963_FUSE_ROM) < 0){
    return -16;
  }
  delay(100); // long wait between AK8963 mode changes

  // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
  readAK8963Registers(AK8963_HXL,7,_buffer);

  // successful init, return 1
  return 1;
}

/* sets the accelerometer full scale range to values other than default */
int MPU9250::setAccelRange(AccelRange range) {
  // setting the accel range to 8G
  if(writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_8G) < 0){
    return -1;
  }
  _accelScale = G * 8.0f/32767.5f; // setting the accel scale to 8G
  _accelRange = range;
  return 1;
}


/* reads the most current data from MPU9250 and stores in buffer */
int MPU9250::readSensor() {
  // grab the data from the MPU9250
  if (readRegisters(ACCEL_OUT, 21, _buffer) < 0) {
    return -1;
  }
  // combine into 16 bit values
  _axcounts = (((int16_t)_buffer[0]) << 8) | _buffer[1];  
  _aycounts = (((int16_t)_buffer[2]) << 8) | _buffer[3];
  _azcounts = (((int16_t)_buffer[4]) << 8) | _buffer[5];
  _tcounts = (((int16_t)_buffer[6]) << 8) | _buffer[7];
  // transform and convert to float values
  _ax = (((float)(tX[0]*_axcounts + tX[1]*_aycounts + tX[2]*_azcounts) * _accelScale) - _axb)*_axs;
  _ay = (((float)(tY[0]*_axcounts + tY[1]*_aycounts + tY[2]*_azcounts) * _accelScale) - _ayb)*_ays;
  _az = (((float)(tZ[0]*_axcounts + tZ[1]*_aycounts + tZ[2]*_azcounts) * _accelScale) - _azb)*_azs;
  return 1;
}

/* returns the accelerometer measurement in the x direction, m/s/s */
float MPU9250::getAccelX_mss() {
  return _ax;
}

/* returns the accelerometer measurement in the y direction, m/s/s */
float MPU9250::getAccelY_mss() {
  return _ay;
}

/* returns the accelerometer measurement in the z direction, m/s/s */
float MPU9250::getAccelZ_mss() {
  return _az;
}

/* writes a byte to MPU9250 register given a register address and data */
int MPU9250::writeRegister(uint8_t subAddress, uint8_t data){
  /* write data to device */
  _i2c->beginTransmission(_address); // open the device
  _i2c->write(subAddress); // write the register address
  _i2c->write(data); // write the data
  _i2c->endTransmission();

  delay(10);
  
  /* read back the register */
  readRegisters(subAddress,1,_buffer);
  /* check the read back register against the written register */
  if(_buffer[0] == data) {
    return 1;
  }
  else{
    return -1;
  }
}

/* reads registers from MPU9250 given a starting register address, number of bytes, and a pointer to store data */
int MPU9250::readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest){
  _i2c->beginTransmission(_address); // open the device
  _i2c->write(subAddress); // specify the starting register address
  _i2c->endTransmission(false);
  _numBytes = _i2c->requestFrom(_address, count); // specify the number of bytes to receive
  if (_numBytes == count) {
    for(uint8_t i = 0; i < count; i++){ 
      dest[i] = _i2c->read();
    }
    return 1;
  } else {
    return -1;
  }
}

/* writes a register to the AK8963 given a register address and data */
int MPU9250::writeAK8963Register(uint8_t subAddress, uint8_t data){
  // set slave 0 to the AK8963 and set for write
	if (writeRegister(I2C_SLV0_ADDR,AK8963_I2C_ADDR) < 0) {
    return -1;
  }
  // set the register to the desired AK8963 sub address 
	if (writeRegister(I2C_SLV0_REG,subAddress) < 0) {
    return -2;
  }
  // store the data for write
	if (writeRegister(I2C_SLV0_DO,data) < 0) {
    return -3;
  }
  // enable I2C and send 1 byte
	if (writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | (uint8_t)1) < 0) {
    return -4;
  }
	// read the register and confirm
	if (readAK8963Registers(subAddress,1,_buffer) < 0) {
    return -5;
  }
	if(_buffer[0] == data) {
  	return 1;
  } else{
  	return -6;
  }
}

/* reads registers from the AK8963 */
int MPU9250::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest){
  // set slave 0 to the AK8963 and set for read
	if (writeRegister(I2C_SLV0_ADDR,AK8963_I2C_ADDR | I2C_READ_FLAG) < 0) {
    return -1;
  }
  // set the register to the desired AK8963 sub address
	if (writeRegister(I2C_SLV0_REG,subAddress) < 0) {
    return -2;
  }
  // enable I2C and request the bytes
	if (writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | count) < 0) {
    return -3;
  }
	delay(1); // takes some time for these registers to fill
  // read the bytes off the MPU9250 EXT_SENS_DATA registers
	_status = readRegisters(EXT_SENS_DATA_00,count,dest); 
  return _status;
}

/* gets the MPU9250 WHO_AM_I register value, expected to be 0x71 */
int MPU9250::whoAmI(){
  // read the WHO AM I register
  if (readRegisters(WHO_AM_I,1,_buffer) < 0) {
    return -1;
  }
  // return the register value
  return _buffer[0];
}

/* gets the AK8963 WHO_AM_I register value, expected to be 0x48 */
int MPU9250::whoAmIAK8963(){
  // read the WHO AM I register
  if (readAK8963Registers(AK8963_WHO_AM_I,1,_buffer) < 0) {
    return -1;
  }
  // return the register value
  return _buffer[0];
}
