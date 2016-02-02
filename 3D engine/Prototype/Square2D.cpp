#include "Square2D.h"

Square2D::Square2D()
{
	this->VBO = 0;
	this->EBO = 0;

	vertexData = new float[16]{
		// x     y      u      v
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f };

	vertexIndices = new GLuint[6]{
		0, 1, 2,
		2, 3, 0 };
}

Square2D::~Square2D()
{
	if (!vertexData == NULL)
		delete[] vertexData;
}

void Square2D::InitRenderData()
{
	if (VBO == 0)
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}
	
	vertexData[0] = (float)x;
	vertexData[4] = (float)x;
	vertexData[8] = (float)(x + width);
	vertexData[12] = (float)(x + width);
	vertexData[1] = (float)y;
	vertexData[5] = (float)(y + height);
	vertexData[9] = (float)(y + height);
	vertexData[13] = (float)y;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, vertexIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Square2D::SetPosition(int x, int y)
{
	this->x = x - (this->width/2) ;
	this->y = y - (this->height / 2);

}

void Square2D::SetSize(int w, int h)
{
	this->width = w;
	this->height = h;
}

void Square2D::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}