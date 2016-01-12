#include "Square.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace Shapes;

Square::Square()
{
	// Set default position, rotation and scaling values
	xPosition = 0.0f;
	yPosition = 0.0f;
	zPosition = 0.0f;

	xRotation = 0.0f;
	yRotation = 0.0f;
	zRotation = 0.0f;

	xScaling = 1.0f;
	yScaling = 1.0f;

	// Set default vertex buffer data
	vertices = new float[32]{
	//  x     y     z      r     g     b       u     v
		1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.5f,   1.0f, 0.0f,
		1.0f,-1.0f, 0.0f,  0.0f, 1.0f, 0.5f,   1.0f, 1.0f,
	   -1.0f,-1.0f, 0.0f,  0.0f, 0.0f, 0.5f,   0.0f, 1.0f,
	   -1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   0.0f, 0.0f };

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	UpdateVBO();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Square::~Square()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete[] vertices;
}

void Square::SetPosition(float x, float y, float z)
{
	xPosition = x;
	yPosition = y;
	zPosition = z;
	glm::vec4 v1 = glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
	glm::vec4 v2 = glm::vec4(vertices[8], vertices[9], vertices[10], 1.0f);
	glm::vec4 v3 = glm::vec4(vertices[16], vertices[17], vertices[18], 1.0f);
	glm::vec4 v4 = glm::vec4(vertices[24], vertices[25], vertices[26], 1.0f);

	glm::mat4 transMatrix;
	transMatrix = glm::translate(transMatrix, glm::vec3(x, y, z));
	v1 = transMatrix*v1;
	v2 = transMatrix*v2;
	v3 = transMatrix*v3;
	v4 = transMatrix*v4;

	vertices[0] = v1.x;
	vertices[1] = v1.y;
	vertices[2] = v1.z;

	vertices[8] = v2.x;
	vertices[9] = v2.y;
	vertices[10] = v2.z;

	vertices[16] = v3.x;
	vertices[17] = v3.y;
	vertices[18] = v3.z;

	vertices[24] = v4.x;
	vertices[25] = v4.y;
	vertices[26] = v4.z;

	UpdateVBO();
}
void Square::SetScaling(float x, float y)
{
	xScaling = x;
	yScaling = y;

	glm::vec4 v1 = glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
	glm::vec4 v2 = glm::vec4(vertices[8], vertices[9], vertices[10], 1.0f);
	glm::vec4 v3 = glm::vec4(vertices[16], vertices[17], vertices[18], 1.0f);
	glm::vec4 v4 = glm::vec4(vertices[24], vertices[25], vertices[26], 1.0f);

	glm::mat4 scaleMatrix;
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(xScaling, yScaling, 1.0f));
	v1 = scaleMatrix*v1;
	v2 = scaleMatrix*v2;
	v3 = scaleMatrix*v3;
	v4 = scaleMatrix*v4;

	vertices[0] = v1.x;
	vertices[1] = v1.y;
	vertices[2] = v1.z;

	vertices[8] = v2.x;
	vertices[9] = v2.y;
	vertices[10] = v2.z;

	vertices[16] = v3.x;
	vertices[17] = v3.y;
	vertices[18] = v3.z;

	vertices[24] = v4.x;
	vertices[25] = v4.y;
	vertices[26] = v4.z;

	UpdateVBO();
}
void Square::SetRotation(float x, float y, float z)
{
	xRotation = x;
	yRotation = y;
	zRotation = z;

	glm::vec4 v1 = glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
	glm::vec4 v2 = glm::vec4(vertices[8], vertices[9], vertices[10], 1.0f);
	glm::vec4 v3 = glm::vec4(vertices[16], vertices[17], vertices[18], 1.0f);
	glm::vec4 v4 = glm::vec4(vertices[24], vertices[25], vertices[26], 1.0f);

	glm::mat4 rotationMatrix;
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));

	v1 = rotationMatrix*v1;
	v2 = rotationMatrix*v2;
	v3 = rotationMatrix*v3;
	v4 = rotationMatrix*v4;

	vertices[0] = v1.x;
	vertices[1] = v1.y;
	vertices[2] = v1.z;

	vertices[8] = v2.x;
	vertices[9] = v2.y;
	vertices[10] = v2.z;

	vertices[16] = v3.x;
	vertices[17] = v3.y;
	vertices[18] = v3.z;

	vertices[24] = v4.x;
	vertices[25] = v4.y;
	vertices[26] = v4.z;

	UpdateVBO();
}
void Square::SetColor(float r, float g, float b)
{
	for (int i = 3; i < 32; i += 8)
	{
		vertices[i] = r;
		vertices[i + 1] = g;
		vertices[i + 2] = b;
	}
}
void Square::MatchTextureToScale()
{
	for (int i = 6;i < 32;i += 8)
	{
		vertices[i] *= xScaling;
		vertices[i + 1] *= yScaling;

		UpdateVBO();
	}
}

void Square::UpdateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Square::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}