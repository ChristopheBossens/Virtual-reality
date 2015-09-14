function [ output_args ] = SendConfigurations( configurations, localServer )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
configurationCount = length(configurations);

display(configurationCount);
display(class(configurations));

fwrite(localServer,1,'uint8');
fwrite(localServer,configurationCount,'int16');

for configurationIndex = 1:configurationCount
    display(configurations{configurationIndex})
    fwrite(localServer,length(configurations{configurationIndex}),'int16');
    fwrite(localServer,configurations{configurationIndex},'char');
end

end

