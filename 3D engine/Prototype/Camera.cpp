#include "Camera.h"

Camera::Camera()
{
	cameraPosition = glm::vec3(0.0f, -0.5f, 80.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = -90.0f;
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
}
void Camera::SetPosition(float x, float y, float z)
{
	cameraPosition = glm::vec3(x, y, z);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}