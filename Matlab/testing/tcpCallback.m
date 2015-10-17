function [ output_args ] = tcpCallback( obj, event )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    firstByte = fread(obj,1,'uint8');
    secondByte = fread(obj,1,'uint8');
    
    fwrite(obj,[7 7],'uint8');

end

