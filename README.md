# Virtual-reality
Overview

-- 3D Engine
Contains C++ code for the running ball software

-- Matlab
Contains scripts useful for analysing motion sensor data (Analysis folder), Matlab classes for Motion sensor, Reward delivery system and Cheetah Daq interaction (Tools folder) and Matlab scripts that provide an interface for generating XML files for running ball experiments (xmlGenerator folder).

-- Tools
  MotionSensor: Contains the arduino code for the motion sensor. Contains an additional program which can be used to quickly check if motion sensor data is recorder correctly
  
  Relay: Program that allows to quickly check if each channel of the relay board is working properly. Can also be used to finetune the amount of time the relay needs to be open to give the proper amount of water
  
  USB1208: Program that allows to quickly manipulate the digital I/O channels of the USB1208 device
