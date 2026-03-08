#include "PerspectiveCamera.h"

namespace Monsi {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_ProjectionMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ViewProjectionMatrix(glm::mat4(1.0f)),
        m_Position({ 0.0f, 0.0f, 3.0f }), m_Front({ 0.0f, 0.0f, -1.0f }), m_Up({ 0.0f, 1.0f, 0.0f })
    {
        ENGINE_PROFILER_FUNCTION();

        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

	void PerspectiveCamera::SetDirection(const glm::vec3& front, const glm::vec3& up)
	{
		m_Front = front;
		m_Up = up;
		RecalculateViewMatrix();
	}

    void PerspectiveCamera::RecalculateViewMatrix()
    {
        ENGINE_PROFILER_FUNCTION();

        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}