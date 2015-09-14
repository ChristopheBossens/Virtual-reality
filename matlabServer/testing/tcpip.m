%% Test script for communicating with Matlab via TCP/IP
% Possible optimization, work out a function for retrieving packets for
% large files so that there is no need to specify an arbitrarily large
% input or output buffer size

% Set up local host
localServer = tcpip('127.0.0.1',6789,'NetworkRole','Server');
localServer.InputBufferSize = 2048;
localServer.OutputBufferSize = 1024;
localServer.BytesAvailableFcn = {'ParseMessages'};
%localServer.BytesAvailableFcnMode = 'byte';
%localServer.BytesAvailableFcnCount = 1;
localServer.ByteOrder = 'littleEndian';
display('Server open and awaiting connection')
fopen(localServer);
display('Client connected.')


fclose(localServer)
% Clean up everything

test = imread('C:\VR_SYSTEM\Stimuli\stimulus_2.png');

data = [];

while(localServer.BytesAvailable > 0)
    data = [data; fread(localServer,localServer.BytesAvailable)];
    pause(0.05);
end

[savepng, mes] = fopen('C:\VR_SYSTEM\Stimuli\test.png','w+')
fwrite(savepng,data)
fclose(savepng)