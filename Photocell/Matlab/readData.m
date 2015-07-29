% Script that demonstrates how to read out the data collected from the
% photocol
filename = 'C:\VR_SYSTEM\Data\sensor.bin';
fid = fopen(filename);
A = fread(fid);
fclose(fid);