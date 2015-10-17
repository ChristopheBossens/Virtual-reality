% Demo_server: scripts that runs the experiment manager as a server. Signal
% to start the experiment must come from an external client
clc;clear all;
experimentManager = ExperimentManager();
experimentManager.InitScreen(0);
experimentManager.InitMotionSensor('COM8',9,31);
experimentManager.InitRewardDelivery(0,4);

matlabServer = tcpip('0.0.0.0',6789,'NetworkRole','Server');
matlabServer.InputBufferSize = 2048;
matlabServer.OutputBufferSize = 1024;

matlabServer.BytesAvailableFcn = {'ParseMessages', experimentManager};
matlabServer.ByteOrder = 'littleEndian';
display('Server open and awaiting connection')
fopen(matlabServer);
display('Client connected.')

