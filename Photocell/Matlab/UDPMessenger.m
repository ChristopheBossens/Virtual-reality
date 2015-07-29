% UDP messenger
% Sends UDP message to Winform UDP server

destPort = 6789;
localPort = 6788;
client = udp('127.0.0.1',destPort);

fopen(client)
fwrite(client,20)
fclose(client)