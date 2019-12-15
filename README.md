# Reflection

## Introduction
This is a course final project of [Intro to Physical Computation], by <b>*Fanyi Pan*</b> and <b>*Yongkun Li*</b>.

## System Diagram
![System Diagram](DocMedia/System_Diagram.jpg)
> System Diagram

## Tech Details

### Bluetooth network
Our system utilize 5 Arduino NANO as separate bluetooth devices, one as the control center and the other 4 as sensor end connected to the central device. Due to the amount limitation of Arduino NANO's Bluetooth power, instead of connect all 4 peripheral devices to the central device at the same time (which is of "Star" topological structure), we use "Serial" shaped network structure. The structure is shown as the following.

![Serial Network](DocMedia/Serial_Network.jpg)
>Serial Network

As shown in the picture, we adopted a kind of method similar to 


### Kinect Position Tracking


### LED color design


## Credit
Thanks <b>*Sohaila Mosbeh*</b> for help of light design.

Thanks <b>*Helen Zegarra*</b> and <b>*Nicole Ginelli*</b> for help of high current issues in LED strip.

Thanks <b>*Rebecca Zhou*</b> for help of smoothing the LED animation. 
