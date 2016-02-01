#version 330 core
// gratingParameters is used to pass specific parameters to the shader
// [0] = grating orientation
// [1] = grating frequency
// [2] = width of the gaussian envelope
// [3] = sine wave phase offset
layout (location = 0) in vec4 vertex;

out vec2 textureCoordinates;
out vec2 rotationVector;
out float envelopeWidth;
out float phaseOffset;

uniform mat4 projection;
uniform vec4 gratingParameters;

void main()
{
	rotationVector = vec2(cos(gratingParameters.x),sin(gratingParameters.y)) * gratingParameters.y;
	envelopeWidth = -0.5/(gratingParameters.z*gratingParameters.z);
	phaseOffset = gratingParameters.w;
	textureCoordinates = vertex.zw + vec2(-0.5, -0.5);
	gl_Position = projection * vec4(vertex.xy,0.0, 1.0);
}