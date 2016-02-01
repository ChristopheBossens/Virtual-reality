#version 330 core
in vec2 textureCoordinates;
in vec2 rotationVector;
in float apertureSize;
in float phaseOffset;

out vec4 color;

void main()
{
	
	float sineWave = sin(dot(rotationVector,textureCoordinates) + phaseOffset);
	float envelope = dot(textureCoordinates,textureCoordinates);
	if (envelope < (0.3*0.3))
		envelope = 1.0;
	else
		envelope = 0.0;
	float fragmentColor = (1.0 + sineWave*envelope)/2.0;
	color = vec4(fragmentColor,fragmentColor,fragmentColor,1.0);
}