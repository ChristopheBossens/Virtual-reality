figure('color','white')

xValues = 1:100;
yValues = ones(1,length(xValues));
plotHandle = plot(xValues,yValues);

% Initialize and configure arduino serial connection
clear arduinoSerial;
arduinoSerial = serial('COM3', 'BaudRate', 9600);
arduinoSerial.BytesAvailableFcnMode = 'byte';
arduinoSerial.BytesAvailableFcnCount = 400;
arduinoSerial.BytesAvailableFcn = {@SerialDataReceived, plotHandle};


% Perform timing test    
global packetsReceived;
global RX;
packetsReceived = 0;

tic
fopen(arduinoSerial);

RX = 0;
fclose(arduinoSerial);
d = toc;
