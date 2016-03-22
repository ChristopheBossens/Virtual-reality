#pragma once
/* Generic class for working with images in orthographic projection mode
   squares can be used to apply textures to for image drawing. Alternatively
   a shader program can be written to do some fancy stimulus manipulation stuff
   on the GPU
*/
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class StimulusQuad
{
private:
	int top, left;
	int width, height;

	GLfloat* vertexData;
	GLuint* vertexIndices;
	GLuint VBO, EBO;
public:
	StimulusQuad();
	~StimulusQuad();

	void InitRenderData();
	void SetCenter(int x, int y);
	void SetPosition(int x, int y);
	void SetSize(int w, int h);

	void Draw(GLuint texID = 0);

	int GetWidth();
	int GetHeight();
};