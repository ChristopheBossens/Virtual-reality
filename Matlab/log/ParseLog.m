function [ parsedLog ] = ParseLog( filename )
%PARSELOG Parse recorder data
%   This function reads a log file and returns data in a format which is
%   more convenient for data analysis. The function returns a matrix with
%   the following structure:
%   Each row contains data from a single frame
%   Column 1: trialIndex to cross-reference with xml file
%   Column 2: timeStmap

filename = 'C:\VR_SYSTEM\Experiments\vertical_initial\1.log';
fid = fopen(filename);
data = fread(fid);
fclose(fid);

trialStart = strfind(data',[255 255 255 255]);

dataMatrix = [];
dataMatrixIndex = 0;
for trialIndex = 1:length(trialStart)
    % Trial number must match trialIndex
    dataIndex = trialStart(trialIndex);
    trialNumber = typecast(uint8(data(dataIndex+4:dataIndex+5)),'uint16');
    
    if trialIndex == length(trialStart)
        maxIndex = length(data);
    else
        maxIndex = trialStart(trialIndex+1);
    end
    
    if trialNumber ~= trialIndex
        display('Mismatch between expected trial number and actual trial number');
    else
        dataIndex = trialStart(trialIndex)+6;
        while (dataIndex + 26) < maxIndex 
            timestamp = typecast(uint8(data(dataIndex:dataIndex+3)),'single');
            dataIndex = dataIndex + 4;
            xVelocity = typecast(uint8(data(dataIndex:dataIndex+3)),'single');
            dataIndex = dataIndex + 4;
            yVelocity = typecast(uint8(data(dataIndex:dataIndex+3)),'single');
            dataIndex = dataIndex + 4;
            frameIndex= typecast(uint8(data(dataIndex:dataIndex+1)),'uint16');
            dataIndex = dataIndex + 2;
            reward    = data(dataIndex);
            dataIndex = dataIndex + 1;
           
            dataMatrixIndex = dataMatrixIndex + 1;
            dataMatrix(dataMatrixIndex,:) = [trialIndex timestamp xVelocity yVelocity double(frameIndex) double(reward)];
        end
    end

end


subplot(2,2,1),plot(dataMatrix(:,2)),title('Timestamp')
subplot(2,2,2),plot(dataMatrix(:,[3])),title('xVelocity')
subplot(2,2,3),plot(dataMatrix(:,5)),title('Frame Index')
subplot(2,2,4),plot(dataMatrix(:,6)),title('Reward delivery')