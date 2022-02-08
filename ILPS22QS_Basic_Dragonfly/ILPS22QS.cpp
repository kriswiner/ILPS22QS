/*
 * Copyright (c) 2020 Tlera Corp.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimers.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimers in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Tlera Corp, nor the names of its contributors
 *     may be used to endorse or promote products derived from this Software
 *     without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 * 
 * The MMC5983MA is an AEC-Q100 qualified complete 3-axis magnetic sensor with on-chip 
 * signal processing and integrated I2C/SPI bus suitable for use in automotive applications.
 * Couple with the LSM6DSM accel/gyro and ILPS22QB barometer the system provides a compact and
 * accurate absolute orientation estimation engine.
 * 
 * Uses the I2C interface for data transfer.
 * 
 * Intended to be run on a Dragonfly development board:
 * 
 * https://www.tindie.com/products/tleracorp/dragonfly-stm32l47696-development-board/
 *  
 * 
 * Library may be used freely and without limit with attribution.
 */

#include "ILPS22QS.h"
#include "I2Cdev.h"

ILPS22Q::ILPS22Q(I2Cdev* i2c_bus)
{
  _i2c_bus = i2c_bus;
}

uint8_t ILPS22Q::getChipID()
{
  // Read the WHO_AM_I register of the altimeter this is a good test of communication
  uint8_t temp = _i2c_bus->readByte(ILPS22Q_ADDRESS, ILPS22Q_WHOAMI);  // Read WHO_AM_I register for ILPS22Q
  return temp;
}

uint8_t ILPS22Q::status()
{
  // Read the status register of the altimeter  
  uint8_t temp = _i2c_bus->readByte(ILPS22Q_ADDRESS, ILPS22Q_STATUS);   
  return temp;
}

int32_t ILPS22Q::readAltimeterPressure()
{
    uint8_t rawData[3];  // 24-bit pressure register data stored here
    _i2c_bus->readBytes(ILPS22Q_ADDRESS, ILPS22Q_PRESS_OUT_XL, 3, &rawData[0]);  
     int32_t temp = (int32_t) ((int32_t) rawData[2] << 24 | (int32_t) rawData[1] << 16 | rawData[0] << 8);
     return (temp >> 8);
}

int16_t ILPS22Q::readAltimeterTemperature()
{
    uint8_t rawData[2];  // 16-bit pressure register data stored here
    _i2c_bus->readBytes(ILPS22Q_ADDRESS, ILPS22Q_TEMP_OUT_L, 2, &rawData[0]);  
    return (int16_t)((int16_t) rawData[1] << 8 | rawData[0]);
}

void ILPS22Q::reset()
{
    uint8_t temp = _i2c_bus->readByte(ILPS22Q_ADDRESS, ILPS22Q_CTRL_REG2);  
    _i2c_bus->writeByte(ILPS22Q_ADDRESS, ILPS22Q_CTRL_REG2, temp | 0x04);  // set software reset bit to reset device

}

void ILPS22Q::Init(uint8_t PODR, uint8_t AVG, uint8_t MODE, uint8_t LPF)
{
    _i2c_bus->writeByte(ILPS22Q_ADDRESS, ILPS22Q_IC3_IF_CTRL, 0x02);  // disable internal CS pullup
    _i2c_bus->writeByte(ILPS22Q_ADDRESS, ILPS22Q_CTRL_REG1, PODR << 3 | AVG);  
  // enable low-pass filter by setting bit 4 to one
  // bit 5 == 0 means bandwidth is odr/4, bit 5 == 1 means bandwidth is odr/4
  // make sure data not updated during read by setting block data udate (bit 3) to 1    
    _i2c_bus->writeByte(ILPS22Q_ADDRESS, ILPS22Q_CTRL_REG2, MODE << 6 | LPF << 5 | 0x10 | 0x08);     
    _i2c_bus->writeByte(ILPS22Q_ADDRESS, ILPS22Q_CTRL_REG3, 0x01);  // enable auto increment of register addresses
}
