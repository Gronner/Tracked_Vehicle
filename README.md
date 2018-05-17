# Tracked Vehicle
This repository contains the code I have written for my tracked vehicle project.

## Goal
The main goal is to learn about different technologies and techniques I was interested in, as well as adding an example to the web, how the Standard Peripheral Drivers Library, provided by ST can be used to interact with the on-board peripherals of the STM32F411-Discovery Board.
In the end a steerable and to a certain level autonomous tracked vehicle is the goal.

## Technologies and Techniques
* All code running on the chip will be written in C
* The chip used is a [STM32F411VE](www.st.com/en/microcontrollers/stm32f411.html?querycriteria=productId=LN1877) which is based on the Cortex-M4 architecture from ARM
* The controller itself is integrated in the [STM32F411E Discovery Board](http://www.st.com/en/evaluation-tools/32f411ediscovery.html)
* The compiler used is the [ARM version of GCC](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)
* All unit test are written in C++ using the [CppUTest Framework](http://cpputest.github.io/)
* Flashing the chip is done via the [ST-Link utility](http://www.st.com/en/development-tools/st-link-v2.html) provided by STMicroelectronics
* [FreeRTOS](https://www.freertos.org/) is used as a real-time operating system which is also implemented in C

To achive the goal of the project the following task need to accomplished:
* Control two DC motors (makeing use of the L298N motor drive H-Bridge)
* Interface with the user with on board LEDs, a 16x2 LCD screen, an IR remote and the USART interface
* Have a closed feedback loop controlling motor voltage, motor speed and vehicle position
* To achive this interact with the accelerometer on the Discovery Board via I2C, the gyroscope via SPI and the STM32F411's on board ADC
* If possible interface with a GPS chip to achive absolute positioning
* Achive a collision detection using ultra sonic sensors


