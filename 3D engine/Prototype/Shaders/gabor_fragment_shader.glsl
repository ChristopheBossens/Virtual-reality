#version 330 core
in vec2 textureCoordinates;
in vec2 rotationVector;
in float envelopeWidth;
in float phaseOffset;

out vec4 color;


void main()
{
	
	float sineWave = sin(dot(rotationVector,textureCoordinates) + phaseOffset);
	float envelope = exp(dot(textureCoordinates,textureCoordinates) * envelopeWidth);
	float fragmentColor = (1.0 + sineWave*envelope)/2.0;
	color = vec4(fragmentColor,fragmentColor,fragmentColor,1.0);
}