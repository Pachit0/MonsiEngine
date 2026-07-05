// SceneCamera.cpp
#include "MonsiPch.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Monsi {

	SceneCamera::SceneCamera()
	{
		RecalcProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalcProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalcProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (height == 0) return;
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalcProjection();
	}

	void SceneCamera::RecalcProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float left = -right;
			float top = m_OrthographicSize * 0.5f;
			float bottom = -top;
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}
	}
}