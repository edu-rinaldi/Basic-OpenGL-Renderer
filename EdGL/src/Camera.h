#pragma once
#include <glm/glm.hpp>

class Camera
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_Up;

	float m_Yaw, m_Pitch, m_Roll;

	float m_MovementSpeed, m_MouseSensitivity, m_FastSpeedMultiplier;

public:
	const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	enum CameraMovement {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};

	Camera() = delete;
	Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up,
			float yaw = -90.f, float pitch = 0, float roll = 0);
	
	
	void Move(const CameraMovement& cm, float dt, bool fastSpeedMultiplier =false);
	void Rotate(const glm::vec3& offset, float dt, bool constrainPitch = true);

	glm::mat4 GetViewMatrix() const;

	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetFront() const { return m_Front; }
	const glm::vec3& GetRight() const { return m_Right; }
	const glm::vec3& GetUp() const { return m_Up; }
private:
	void updateCameraVectors();


};

