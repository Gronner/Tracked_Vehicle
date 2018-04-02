---
layout: post
title:  "Architecture of the Tracked Vehicle"
date:   2018-04-02 18:46:10 +0200
categories: jekyll update
---
[//]: <> Add Tex Math environment
<script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>

## Hardware
![Block diagram of the planed tracked vehicle]({{ "/assets/img/HW_SW_Block_Diagram.svg" | absolute_url }})

In the block diagram above you can see the first idea of how the vehicle's software, actuator and sensor architecture could look like in the end.
The end goal is to have a closed loop control for the vehicle's position.
Prerequisites for this are to know at least the relative position of the vehicle after $$t_0$$ and the speed it is moving and turning during the time steps.
This can be done by using the accelerometer and the gyroscope already present on the board.
For a better control quality additional speed sensors, measuring the rotational speed of the DC motors, can be used.
While this allows the car to be driven in a closed loop control, obstacles in the drive path need to be sensed.
From a sensor set I do have an ultrasonic sensor lying around, which I'll be using for the task of obstacle sensing.

For a more advanced positioning a GPS module could be used.
Some examples I took a look at made use of the SPI bus to communicate with a microcontroller,
which is why I placed it there in the block diagram.
Depending on the progress of the project and the necessity of it, I'll try to incorporate a GPS module.

To have a form of output from the microcontroller I'll first make use of the on-board LEDs and will later add a LCD to output text.
If the need arises I'll make us of the USART interface to communicate via a serial interface with my computer. 

To give input to the controller my current plan is to use an infrared sensor and an infrared remote I have lying around.
The sensor uses [pulse-code-modulation](https://en.wikipedia.org/wiki/Pulse-code_modulation) to transfer the remote's button information, which then can be read and interpreted by the microcontroller.
The board also has one user and one reset button.
The addition of other buttons, should they be necessary, can easily be done later on.

## Software
From the software point of view, any of these hardware building blocks will need low drivers.
These will be written using the Standard Peripheral Library provided by STMicroelectronics.
On top of that some signal processing will be necessary.

Closed loop control controllers for position, motor speed and the driving voltage, controlled by the PWM signal, are necessary.
Wether these will be PIDs or maybe even a state controller will be determined later.
For the position a synthesis of the gyroscope and accelerometer data will be needed.

To organize and time all these tasks a real-time operating will be necessary, especially for guaranteeing the strict timing needs for the controllers. 
As mentioned in the previous post, I'll be using FreeRTOS for this task.
With this a task system needs to be developed which can integrate the sensors, the controllers and various outputs together.

All code can be found at [my repository](https://github.com/Gronner/Tracked_Vehicle).
