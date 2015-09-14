function [ returnList ] = GetAvailableConfigurations( )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
experimentPath = 'C:\VR_SYSTEM\Experiments\';

availableFolders = dir([experimentPath '*.']);

if size(availableFolders,1) > 2
    returnList = cell(1,size(availableFolders,1)-2);
    for folderIndex = 3:size(availableFolders,1)
        returnList{folderIndex-2} = availableFolders(folderIndex).name;
    end
else
    returnList = [];
end

end

