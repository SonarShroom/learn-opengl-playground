#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Scene
{

class Camera
{
public:
	enum class CameraMode
	{
		ORTHOGRAPHIC = 0,
		PERSPECTIVE,
		SIZE
	};

	static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	constexpr glm::vec3 GetPosition() const { return position; }
	constexpr void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
	void MoveCamera(const glm::vec3 direction);

	constexpr glm::vec3 GetEulerRotation() const { return eulerRotation; }
	void SetEulerRotation(const glm::vec3& newRotation);
	void Rotate(const glm::vec2 rotationDirection);
	
	constexpr glm::vec3 GetForward() const { return forward; }
	constexpr void SetForward(const glm::vec3& newForward) { forward = newForward; }
	
	constexpr float GetSpeed() const { return speed; }
	constexpr void SetSpeed(const float newSpeed) { speed = newSpeed; }

	constexpr float GetFoV() const { return fieldOfView; }
	void SetFoV(const float newFoV);
	void Zoom(const float fovDiff);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
	CameraMode mode = CameraMode::PERSPECTIVE;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 eulerRotation = glm::vec3(0.0f, -90.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	float speed = 10.0f;
	float near = 0.1f;
	float far = 100.0f;

	float fieldOfView = 45.0f;

	glm::vec2 viewportSize = glm::vec2(800.0f, 600.0f);
};

}