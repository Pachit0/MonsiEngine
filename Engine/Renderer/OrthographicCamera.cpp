#include "MonsiPch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Monsi {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom = -1.0f, float top = 1.0f)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
		ENGINE_PROFILER_FUNCTION();
		m_ViewPorjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		ENGINE_PROFILER_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewPorjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position) {
		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation) {
		m_Rotation = rotation; 
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		ENGINE_PROFILER_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		
		m_ViewMatrix = glm::inverse(transform);
		m_ViewPorjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}