function [ name, value ] = xmlParseAttribute( attributeList)
%xmlParseAttribute
% returns arrays with name and values of each attribute in attributeList
attributeCount = attributeList.getLength;

name = cell(1,attributeCount);
value= cell(1,attributeCount);

for attributeIndex = 0:(attributeCount-1)
    strAttribute = char(item(attributeList,attributeIndex));
    
    sep = strfind(strAttribute,'=');
    name{attributeIndex+1} = strAttribute(1:(sep-1));
    value{attributeIndex+1} = strAttribute((sep+2):(end-1));
end

if attributeCount == 1
    name = char(name);
    value = char(value);
end
end

