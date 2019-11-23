/*
MPU9250.h
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

#ifndef MPU9250_h
#define MPU9250_h

#include "Arduino.h"
#include "Wire.h"    // I2C library

class MPU9250{
  public:
    enum AccelRange
    {
      ACCEL_RANGE_8G
    };

    MPU9250(TwoWire &bus,uint8_t address);
    int begin();
    int setAccelRange(AccelRange range);
    int readSensor();
    float getAccelX_mss();
    float getAccelY_mss();
    float getAccelZ_mss();

  protected:
    // i2c
    uint8_t _address;
    TwoWire *_i2c;
    const uint32_t _i2cRate = 400000; // 400 kHz
    size_t _numBytes; // number of bytes received from I2C
    // track success of interacting with sensor
    int _status;
    // buffer for reading from sensor
    uint8_t _buffer[9];
    // data counts
    int16_t _axcounts,_aycounts,_azcounts;
    int16_t _tcounts;
    // data buffer
    float _ax, _ay, _az;
    float _t;
    // scale factors
    float _accelScale;

    // configuration
    AccelRange _accelRange;
    // accel bias and scale factor estimation
    float _axb, _ayb, _azb;
    float _axs = 1.0f;
    float _ays = 1.0f;
    float _azs = 1.0f;

    // transformation matrix
    /* transform the accel and gyro axes to match the magnetometer axes */
    const int16_t tX[3] = {0,  1,  0}; 
    const int16_t tY[3] = {1,  0,  0};
    const int16_t tZ[3] = {0,  0, -1};
    // constants
    const float G = 9.807f;
    // MPU9250 registers
    const uint8_t ACCEL_OUT = 0x3B;
    const uint8_t EXT_SENS_DATA_00 = 0x49;
    const uint8_t ACCEL_CONFIG = 0x1C;
    const uint8_t ACCEL_FS_SEL_8G = 0x10;
    const uint8_t CONFIG = 0x1A;
    const uint8_t SMPDIV = 0x19;
    const uint8_t PWR_MGMNT_2 = 0x6C;
    const uint8_t SEN_ENABLE = 0x00;
    const uint8_t USER_CTRL = 0x6A;
    const uint8_t I2C_MST_EN = 0x20;
    const uint8_t I2C_MST_CLK = 0x0D;
    const uint8_t I2C_MST_CTRL = 0x24;
    const uint8_t I2C_SLV0_ADDR = 0x25;
    const uint8_t I2C_SLV0_REG = 0x26;
    const uint8_t I2C_SLV0_DO = 0x63;
    const uint8_t I2C_SLV0_CTRL = 0x27;
    const uint8_t I2C_SLV0_EN = 0x80;
    const uint8_t I2C_READ_FLAG = 0x80;
    const uint8_t WHO_AM_I = 0x75;
    // AK8963 registers
    const uint8_t AK8963_I2C_ADDR = 0x0C;
    const uint8_t AK8963_HXL = 0x03; 
    const uint8_t AK8963_CNTL1 = 0x0A;
    const uint8_t AK8963_PWR_DOWN = 0x00;
    const uint8_t AK8963_FUSE_ROM = 0x0F;
    const uint8_t AK8963_CNTL2 = 0x0B;
    const uint8_t AK8963_RESET = 0x01;
    const uint8_t AK8963_WHO_AM_I = 0x00;
    // private functions
    int writeRegister(uint8_t subAddress, uint8_t data);
    int readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest);
    int writeAK8963Register(uint8_t subAddress, uint8_t data);
    int readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest);
    int whoAmI();
    int whoAmIAK8963();
};

#endif
