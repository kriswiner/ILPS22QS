# ILPS22QS
High-accuracy barometer and QVAR (touch) sensor

Latest ST Microelectronics barometer with embedded QVAR touch sensing.

The [ILPS22QS](https://www.st.com/resource/en/datasheet/ilps22qs.pdf) barometer has a FIFO as well as low-pass filter and averaging which promises to provide even more accuracy than its predecesor. the LPS22HB. Uncalibrated absolute pressure accuracy of 0.5 hPa (0.5 mBar) and low-pressure noise of 0.34 Pa while allowing for data rates from 1 to 200 Hz. Compare with the LPS22HS: 1 hPa uncalibrated absolute pressure accuracy, 0.75 Pa low-pressure noise with data rates from 1 to 75 Hz.

For man-down applications it is the relative accuracy that matters. The ILPS22QS has a relative accuracy of +/-0.015 hPa compared to +/-0.1 for the LPS22HB (800 - 1100 hPa range at 25 C), almost 7x better. This should allow fine discrimination between even smaller height changes.

The **Basic** sketch shows how to configure the sensor data rate and full-scale range, set the averaging and low-pass filter and poll for data at a fixed rate, and then convert the data into properly scaled pressure (mBar) and temperature (degrees Celsius). An estimate of altitude is made from the scaed pressure data and everything is output to the serial monitor at 1 Hz.

I am using a breakout board I designed for the ILPS22QS whose design is available at the OSH Park shared space [here](https://oshpark.com/shared_projects/6DSsyPEF). I used the STM32L476RE ([Dragonfly](https://www.tindie.com/products/tleracorp/dragonfly-stm32l47696-development-board/)) development board for sketch development and testing.

Here is an example of the kind of output one can achieve (1 Hz, 16x averaging, low-pass filter at ODR/9). On power on it takes ~9 data points to reach stability. Thereafter, with the breadboard flat on the desktop I collected ~50 data points. Tthen raised the board over my head, ~3 feet above the desk top. I collected another 50 data points, then put the board back down on the desktop and collected the final ~50 data points. I am plotting absolute presure and estimated altitude.
