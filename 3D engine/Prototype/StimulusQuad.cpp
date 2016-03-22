#include "StimulusQuad.h"

StimulusQuad::StimulusQuad()
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

	this->top = 0;
	this->left = 0;
	this->width = 10;
	this->height = 10;
}

StimulusQuad::~StimulusQuad()
{
	if (!vertexData == NULL)
		delete[] vertexData;
}

void StimulusQuad::InitRenderData()
{
	if (VBO == 0)
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, vertexIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	vertexData[0] = (float)left;
	vertexData[4] = (float)left;
	vertexData[8] = (float)(left + width);
	vertexData[12] = (float)(left + width);
	vertexData[1] = (float)top;
	vertexData[5] = (float)(top + height);
	vertexData[9] = (float)(top + height);
	vertexData[13] = (float)top;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StimulusQuad::SetCenter(int x, int y)
{
	this->top = y - (height / 2.0f);
	this->left = x - (width / 2.0f);
}
void StimulusQuad::SetPosition(int top, int left)
{
	this->top = top;
	this->left = left;

}

void StimulusQuad::SetSize(int w, int h)
{
	this->width = w;
	this->height = h;
}

void StimulusQuad::Draw(GLuint texID)
{
	if (texID != 0)
		glBindTexture(GL_TEXTURE_2D, texID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int StimulusQuad::GetHeight()
{
	return this->height;
}

int StimulusQuad::GetWidth()
{
	return this->width;
}