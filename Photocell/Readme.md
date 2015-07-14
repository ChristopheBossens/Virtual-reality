# Photoresistor

1) The photocell measures continuously the luminance on the screen (current sensor polling is established at 6000 Hz). 
   In matlab we continously change the screen luminance, sending a synchronization signal to the sensor polling system.
   Afterwards we can analyse this data to get an idea of how the screen responds to commands sent from Matlab. The photocell
   data is registered by a C# application. In C#, it is straightforward to develop multithreaded applications so that data
   collection can easily be combined with a user interface system without blocking it
   
2) We attach an interupt routine to the analog channel of the Arduino, which is fired as soon as the voltage exceeds a threshold
   This interrupt then sends a signal to the system. This can be used to signal stimulus offset and onset
   
   
