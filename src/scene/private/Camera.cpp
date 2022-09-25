#include "scene/Camera.h"

#include <cmath>

namespace Scene
{

constexpr glm::vec3 Camera::WORLD_UP;

void Camera::MoveCamera(const glm::vec3 direction)
{
	position += direction;
}

void Camera::SetEulerRotation(const glm::vec3& newRotation)
{
	eulerRotation = newRotation;
	if (eulerRotation.x > 89.0f)
	{
		eulerRotation.x = 89.0f;
	}
	else if (eulerRotation.x < -89.0f)
	{
		eulerRotation.x = -89.0f;
	}
	
	forward.x = std::cos(glm::radians(eulerRotation.y)) * std::cos(glm::radians(eulerRotation.x));
	forward.y = std::sin(glm::radians(eulerRotation.x));
	forward.z = std::sin(glm::radians(eulerRotation.y)) * std::cos(glm::radians(eulerRotation.x));
	forward = glm::normalize(forward);
}

void Camera::Rotate(const glm::vec2 rotationDirection)
{
	// Y Axis rotation is the yaw, otherwise known as left/right rotation.
	// X Axis rotation is the pitch, otherwise known as up/down rotation.
	SetEulerRotation({eulerRotation.x + rotationDirection.y, eulerRotation.y + rotationDirection.x, eulerRotation.z});
}

void Camera::SetFoV(const float newFoV)
{
	fieldOfView = newFoV;
	if (fieldOfView < 1.0f)
	{
		fieldOfView = 1.0f;
	}
	else if (fieldOfView > 45.0f)
	{
		fieldOfView = 45.0f;
	}
}

void Camera::Zoom(const float fovDiff)
{
	SetFoV(fieldOfView - fovDiff);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, WORLD_UP);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	switch (mode)
	{
		case CameraMode::ORTHOGRAPHIC:
			return glm::ortho(0.0f, viewportSize.x, 0.0f, viewportSize.y, near, far);
			break;
		case CameraMode::PERSPECTIVE:
			return glm::perspective(glm::radians(fieldOfView), viewportSize.x / viewportSize.y, near, far);
			break;
		default:
			break;
	}
	return glm::mat4();
}

}