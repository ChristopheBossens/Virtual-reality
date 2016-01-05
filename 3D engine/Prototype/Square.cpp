#include "Square.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace Shapes;

Square::Square()
{
	GLfloat vertices[] = {
	//  x     y     z      r     g     b       u     v
		1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
		1.0f,-1.0f, 0.0f,  0.0f, 1.0f, 0.5f,   1.0f, 0.0f,
	   -1.0f,-1.0f, 0.0f,  0.0f, 0.0f, 0.5f,   0.0f, 0.0f,
	   -1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   0.0f, 1.0f };
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Square::~Square()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Square::Draw()
{
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}