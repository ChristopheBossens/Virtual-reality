% Compare TCP/IP vs UDP connection speed
%

tcpServer = tcpip('0.0.0.0',6788,'NetworkRole','Server');
tcpServer.ByteOrder = 'littleEndian';
tcpServer.BytesAvailableFcn = {'tcpCallback'};

display('Tcp server open and awaiting connection')
fopen(tcpServer);
display('Client connected');

%%
udpServer = udp('127.0.0.1',5677,'LocalPort',5688);
udpServer.ByteOrder = 'littleEndian';
udpServer.BytesAvailableFcn = {'udpCallback'};
display('UDP server open and awaiting connection');
fopen(udpServer);
display('Client connected');
