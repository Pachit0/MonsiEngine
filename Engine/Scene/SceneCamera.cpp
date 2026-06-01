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
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalcProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);

		RecalcProjection();
	}

	void SceneCamera::RecalcProjection()
	{
		float right = m_OrthographicSize * m_AspectRatio * 0.5f;
		float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float top = m_OrthographicSize * 0.5f;
		float bottom = -m_OrthographicSize * 0.5f;

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
	}

}