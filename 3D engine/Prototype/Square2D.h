#pragma once
/* Generic class for working with images in orthographic projection mode
   squares can be used to apply textures to for image drawing. Alternatively
   a shader program can be written to do some fancy stimulus manipulation stuff
   on the GPU
*/
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Square2D
{
private:
	int x, y;
	int width, height;

	GLfloat* vertexData;
	GLuint* vertexIndices;

	GLuint VBO, EBO;
public:
	Square2D();
	~Square2D();

	void InitRenderData();
	void SetPosition(int x, int y);
	void SetSize(int w, int h);

	void Draw();
};