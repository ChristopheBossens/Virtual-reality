#include "Camera.h"

// Initializer:
// Set default camera position
// Set default projection matrix
// Assigns pointers 
Camera::Camera()
{
	cameraPosition = glm::vec3(0.0f, -0.5f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = -90.0f;

	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	projectionMatrix = glm::perspective(45.0f, 2.0f, 0.1f, 80.0f);

	pViewMatrix = glm::value_ptr(viewMatrix);
	pProjectionMatrix = glm::value_ptr(projectionMatrix);
}

float Camera::GetX()
{
	return cameraPosition.x;
}
float Camera::GetY()
{
	return cameraPosition.y;
}
float Camera::GetZ()
{
	return cameraPosition.z;
}

void Camera::SetX(float x)
{
	this->cameraPosition.x = x;
}
void Camera::SetY(float y)
{
	this->cameraPosition.y = y;
}
void Camera::SetZ(float z)
{
	this->cameraPosition.z = z;
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	pViewMatrix = glm::value_ptr(viewMatrix);
}
void Camera::SetPosition(float x, float y, float z)
{
	cameraPosition = glm::vec3(x, y, z);
}

glm::mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearZ, float farZ)
{
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, 0.1f, 100.0f);
	pProjectionMatrix = glm::value_ptr(projectionMatrix);
}