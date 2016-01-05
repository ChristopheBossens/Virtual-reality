#include "Triangle.h"

using namespace Shapes;

Triangle::Triangle()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f, 0.5f, 1.0f };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Triangle::~Triangle()
{
	glDeleteBuffers(1, &VBO);
}

void Triangle::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}