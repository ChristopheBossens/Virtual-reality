# Virtual-reality
Overview

--matlabServer
Contains the code that runs on the stimulus computer. Contains an xmlParser to read xml trial files, a tcp connection manager to allow interaction from the vrClient program, and an experiment  routine that uses Psychophysics toolbox for stimulus presentation

-- Motion Sensor
Contains arduino sketch for ADNS motion sensor and wiring diagram

-- PhotoCell
Contains arduino sketch for the photocell, as well as analysis scripts for photocell data and calibration data for the current setup.

--VR_Software
Contains the following applications:
  1. Motion sensor: can be used to test data from the motion sensor independent from an experiment. Use this if you suspect something is wrong the motion sensors
  2. Photosensor: This application is used to register data collected when the PhotoCell arduino sketch is running. Use this if you suspect something is wrong with stimulus presentation.
  3. Relay: Application that can be used to quickly test the relay board used for reward delivery
  4. vrClient: GUI application for controlling the matlabServer and delivering PSTH from the Cheetah system

