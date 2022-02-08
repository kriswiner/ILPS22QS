# ILPS22QS
 Latest ST Microelectronics high-accuracy barometer with embedded QVAR touch sensing.

The [ILPS22QS](https://www.st.com/resource/en/datasheet/ilps22qs.pdf) barometer has a FIFO as well as low-pass filter and averaging and provides much higher accuracy than its predecesor, the LPS22HB: uncalibrated absolute pressure accuracy of 0.5 hPa (0.5 mBar) and low-pressure noise of 0.34 Pa while allowing for data rates from 1 to 200 Hz. Compare with the LPS22HS: 1 hPa uncalibrated absolute pressure accuracy, 0.75 Pa low-pressure noise with data rates from 1 to 75 Hz.

For man-down applications it is the relative accuracy that matters. The ILPS22QS has a relative accuracy of +/-0.015 hPa compared to +/-0.1 for the LPS22HB (800 - 1100 hPa range at 25 C), almost 7x better. This should allow excellent discrimination of even small height changes.

The **Basic** sketch shows how to configure the sensor data rate and full-scale range, set the averaging and low-pass filter and poll for data at a fixed rate, and then convert the data into properly scaled pressure (mBar) and temperature (degrees Celsius). An estimate of altitude is made from the scaled pressure data and all three are output to the serial monitor at 1 Hz.

I am using a breakout board for the ILPS22QS whose design is available at the OSH Park shared space [here](https://oshpark.com/shared_projects/6DSsyPEF). I used the STM32L476RE ([Dragonfly](https://www.tindie.com/products/tleracorp/dragonfly-stm32l47696-development-board/)) development board for sketch development and testing.

![breadboard](https://user-images.githubusercontent.com/6698410/152866710-c6c06b74-30ec-4170-b119-8afecbaef402.jpg)

Here is an example of the kind of output one can achieve (*1 Hz, 16x averaging, low-pass filter at ODR/9*). At power-on it takes ~9 data points for the pressure to stabilize. Thereafter, with the breadboard flat on the desktop I collected ~50 data points. Then raised the board over my head, ~3 feet above the desk top. I collected another 50 data points, then put the board back down on the desktop and collected the final ~50 data points. I am plotting absolute presure and estimated altitude.

![pressure](https://user-images.githubusercontent.com/6698410/152865780-8f2dc6a0-c7d4-4eeb-bbbb-1d0bc9609873.jpg)
![altitude](https://user-images.githubusercontent.com/6698410/152865843-5b3621ef-5f8b-4a50-bf40-e741c761cbbd.jpg)

The relative RMS altitude error is ~0.5 foot or so. Plenty good enough to determine whether a person wearing such a sensor has fallen down. At these settings, sensor power usage should be ~3.5 uA (per the data sheet). So excellent relative height discrimination can be achieved at ultra-low power usage. Presumably, at higher data rate and with more averaging (up to 512x) the jitter could be further reduced but at the cost of more power usage.

The ILPS22QS has two output interrupts for pressure threshold alerts which could aid in the detection of "man down" events. But these two pins can also be used as analog inputs for [QVAR](https://www.st.com/resource/en/application_note/an5755-qvar-sensing-channel--stmicroelectronics.pdf) (electric charge variation) using a flex connector with one of several types of [electrodes](https://oshpark.com/shared_projects/Q802dDIE) designed to detect human touch.

The **QVAR** sketch simply enables QVAR in register CTRL_REG3 and outputs the scaled output of the pressure registers as the QVAR signal in mV (full scale +/- 18 mV). The application note (AN5755) indicates that it is possible to interleave pressure and QVAR but I couldn't make this work and this interleave mode is not mentioned in the data sheet. This would be a cool capability.

I designed a simple flex electrode after the guidance in the AN as shown here:

I was able to make use of both analog inputs, one positive and one negative.

![AH1](https://user-images.githubusercontent.com/6698410/152895165-3805de8b-382f-4323-8ee6-ae65ade45b0c.jpg)

The positive data was taken using pin AH1 with the same 1 Hz settings as the pressure data above. When I touched the exposed electrode, it took several seconds for the QVAR signal to max out. There was an offset of ~-1.5 mV which was supposed to be eliminated by the offset circuit I included in my design. Obviously something went wrong. I tried both long touch and tapping. The 1 Hz rate meant that in order to get tapping to register at all it had to be more like short touching. The sluggish respone is apparent in the data.

![AH2](https://user-images.githubusercontent.com/6698410/152895183-ae8ce84d-d44b-4bc9-a59a-1c27e1e3fb29.jpg)

The negative data was taken using pin AH2. This time I changed the data rate to 10 Hz. The response was much crisper and I was able to get something pretty close to a tap to work. For accelerometers, tap detection is only reliable at 200 Hz or so. For QVAR, even 10 Hz seems to work well enough, but maybe 25 Hz would be better. Still, the lower rate one can run the sensor the lower the power usage.

I hae a new flex electrode design coming from OSH Park and next test will be using two electrodes at once, one on each input, to generate positive and negative touch data. After that, I would like to explosre sliding touch sensors to see if more sophisticated touch sensing can be done...
