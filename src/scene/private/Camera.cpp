#include "scene/Camera.h"

#include <cmath>

#include "glm/gtc/matrix_access.hpp"

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

glm::mat4 LookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
{
	auto _inverseDirection = glm::normalize(pos - target);
	auto _rightVector = glm::normalize(glm::cross(worldUp, _inverseDirection));
	auto _upVector = glm::normalize(glm::cross(_inverseDirection, _rightVector));
	
	glm::mat4 _position = glm::mat4(1.0f);
	_position = glm::translate(_position, -pos);
	glm::mat4 _lookAtRotation = glm::mat4(1.0f);
	_lookAtRotation = glm::column(_lookAtRotation, 0, glm::vec4(_rightVector, 0.0f));
	_lookAtRotation = glm::column(_lookAtRotation, 1, glm::vec4(_upVector, 0.0f));
	_lookAtRotation = glm::column(_lookAtRotation, 2, glm::vec4(_inverseDirection, 0.0f));
	_lookAtRotation = glm::transpose(_lookAtRotation);
	return _lookAtRotation * _position;
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