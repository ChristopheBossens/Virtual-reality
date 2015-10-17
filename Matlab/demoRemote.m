% Demo_server: scripts that runs the experiment manager as a server.
% Experiments need to be loaded and started from the vrClient application
try
    clc;clear all;
    experimentManager = ExperimentManager();
    experimentManager.InitScreen(0);
    experimentManager.InitMotionSensor('COM8',9,31);
    experimentManager.InitRewardDelivery(0,2);

    matlabServer = tcpip('0.0.0.0',6789,'NetworkRole','Server');
    matlabServer.InputBufferSize = 2048;
    matlabServer.OutputBufferSize = 1024;

    matlabServer.BytesAvailableFcn = {'ParseMessages', experimentManager};
    matlabServer.ByteOrder = 'littleEndian';
    display('Server open and awaiting connection')
    fopen(matlabServer);
    display('Client connected.')
catch ME
    fclose(matlabServer);
end
