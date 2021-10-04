#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace edgl {

	Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up,
		float yaw, float pitch, float roll) :
		m_Position(position), m_Front(front), m_Up(up),
		m_Yaw(yaw), m_Pitch(pitch), m_Roll(roll),
		m_MovementSpeed(1.5f), m_MouseSensitivity(0.05f), m_FastSpeedMultiplier(2)
	{
		updateCameraVectors();
	}

	void Camera::Move(const CameraMovement& cm, float dt, bool fastSpeedMultiplier)
	{
		float velocity = (fastSpeedMultiplier ? m_MovementSpeed * m_FastSpeedMultiplier : m_MovementSpeed) * dt;
		switch (cm)
		{
		case FORWARD:
			m_Position += velocity * m_Front;
			break;
		case BACKWARD:
			m_Position -= velocity * m_Front;
			break;
		case LEFT:
			m_Position -= velocity * m_Right;
			break;
		case RIGHT:
			m_Position += velocity * m_Right;
			break;
		case UP:
			m_Position += velocity * WORLD_UP;
			break;
		case DOWN:
			m_Position -= velocity * WORLD_UP;
			break;
		default:

			break;
		}
	}

	void Camera::Rotate(const glm::vec3& offset, float dt, bool constrainPitch)
	{
		m_Yaw += -offset.x * m_MouseSensitivity;
		m_Pitch += offset.y * m_MouseSensitivity;

		if (constrainPitch)
		{
			if (m_Pitch > 89.0f)	m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)	m_Pitch = -89.0f;
		}

		updateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 t_Front;
		// Recalculate front, right and up camera "frame" vector
		t_Front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		t_Front.y = glm::sin(glm::radians(m_Pitch));
		t_Front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(t_Front);
		m_Right = glm::normalize(glm::cross(m_Front, WORLD_UP));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

}