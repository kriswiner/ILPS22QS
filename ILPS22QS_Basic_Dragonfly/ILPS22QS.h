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

#ifndef ILPS22QS_h
#define ILPS22QS_h

#include "Arduino.h"
#include <Wire.h>
#include "I2Cdev.h"

// See ILPS22Q "MEMS pressure sensor: 260-1260 hPa absolute digital output barometer" Data Sheet
// http://www.st.com/content/ccc/resource/technical/document/datasheet/bf/c1/4f/23/61/17/44/8a/DM00140895.pdf/files/DM00140895.pdf/jcr:content/translations/en.DM00140895.pdf
#define ILPS22Q_INTERRUPT_CFG 0x0B
#define ILPS22Q_THS_P_L       0x0C
#define ILPS22Q_THS_P_H       0x0D
#define ILPS22Q_WHOAMI        0x0F // should return 0xB4
#define ILPS22Q_CTRL_REG1     0x10
#define ILPS22Q_CTRL_REG2     0x11
#define ILPS22Q_CTRL_REG3     0x12
#define ILPS22Q_FIFO_CTRL     0x14
#define ILPS22Q_FIFO_WTM      0x15
#define ILPS22Q_REF_P_L       0x16
#define ILPS22Q_REF_P_H       0x17
#define ILPS22Q_IC3_IF_CTRL   0x19
#define ILPS22Q_RPDS_L        0x1A
#define ILPS22Q_RPDS_H        0x1B
#define ILPS22Q_INT_SOURCE    0x24
#define ILPS22Q_FIFO_STATUS1  0x25
#define ILPS22Q_FIFO_STATUS2  0x26
#define ILPS22Q_STATUS        0x27
#define ILPS22Q_PRESS_OUT_XL  0x28
#define ILPS22Q_PRESS_OUT_L   0x29
#define ILPS22Q_PRESS_OUT_H   0x2A
#define ILPS22Q_TEMP_OUT_L    0x2B
#define ILPS22Q_TEMP_OUT_H    0x2C
#define ILPS22Q_QVAR_DISABLE  0x5F
#define ILPS22Q_FIFO_DATA_OUT_PRESS_XL   0x78
#define ILPS22Q_FIFO_DATA_OUT_PRESS_L    0x79
#define ILPS22Q_FIFO_DATA_OUT_PRESS_H    0x7A

#define ILPS22Q_ADDRESS 0x5C   // Address of altimeter

// Altimeter output data rate
#define    P_1shot  0x00 // power down/1-shot
#define    P_1Hz    0x01
#define    P_4Hz    0x02
#define    P_10Hz   0x03
#define    P_25Hz   0x04  // 25 Hz output data rate
#define    P_50Hz   0x05
#define    P_75Hz   0x06
#define    P_100Hz  0x07
#define    P_200Hz  0x08

// Pressure, temperature averaging
#define avg_4    0x00
#define avg_8    0x01
#define avg_16   0x02
#define avg_32   0x03
#define avg_64   0x04
#define avg_128  0x05
#define avg_512  0x07

#define mode1 0x00
#define mode2 0x01

#define lpf_odr4 0x00
#define lpf_odr9 0x01

class ILPS22Q
{
  public: 
  ILPS22Q(I2Cdev* i2c_bus);
  void Init(uint8_t PODR, uint8_t AVG, uint8_t MODE, uint8_t LPF);
  uint8_t getChipID();
  uint8_t status();
  void reset();
  int32_t readAltimeterPressure();
  int16_t readAltimeterTemperature();
  private:
    I2Cdev* _i2c_bus;
};

#endif
