#include "MonsiPch.h"
#include "PerspectiveControl.h"
#include "MonsiKeyCodes.h"
#include "Engine.h"

#include "Input.h"
#include <GLFW/glfw3.h>

namespace Monsi {

	PerspectiveControl::PerspectiveControl(float aspectRatio, bool rotationFlag)
		: m_AspectRatio(aspectRatio),
		m_CameraRotationFlag(rotationFlag),
		m_Camera(m_FOV, aspectRatio, m_NearClip, m_FarClip),
		m_CameraPosition({ 0.0f,0.0f,0.3f }),
		m_CameraFront({ 0.0f,0.0f,-1.0f }),
		m_CameraUp({ 0.0f,1.0f,0.0f }),
		m_CameraRight({ 1.0f,0.0f,0.0f })
	{
		UpdateCameraVectors();
		m_Camera.SetPosition(m_CameraPosition);
	}
	PerspectiveControl::PerspectiveControl(const glm::vec3& position, float aspectRatio, bool rotationFlag)
		: m_AspectRatio(aspectRatio),
		m_CameraRotationFlag(rotationFlag),
		m_Camera(m_FOV, aspectRatio, m_NearClip, m_FarClip),
		m_CameraPosition(position),
		m_CameraFront({ 0.0f,0.0f,-1.0f }),
		m_CameraUp({ 0.0f,1.0f,0.0f }),
		m_CameraRight({ 1.0f,0.0f,0.0f })
	{
		UpdateCameraVectors();
		m_Camera.SetPosition(m_CameraPosition);
	}
	PerspectiveControl::PerspectiveControl(const glm::vec3& position, const glm::vec3& direction, float aspectRatio, bool rotationFlag)
		: m_AspectRatio(aspectRatio),
		m_CameraRotationFlag(rotationFlag),
		m_Camera(m_FOV, aspectRatio, m_NearClip, m_FarClip),
		m_CameraPosition(position),
		m_CameraFront({ 0.0f,0.0f,-1.0f }),
		m_CameraUp({ 0.0f,1.0f,0.0f }),
		m_CameraRight({ 1.0f,0.0f,0.0f })
	{
		UpdateCameraVectors();
		m_Camera.SetPosition(m_CameraPosition);
	}

	void PerspectiveControl::OnLayerUpdate(TimeStep ts)
	{
		static bool cPressedLastFrame = false;
		bool cPressedThisFrame = Input::KeyPressed(MONSI_KEY_C);

		if (cPressedThisFrame && !cPressedLastFrame)
		{
			m_MouseCaptured = !m_MouseCaptured;

			GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

			if (m_MouseCaptured)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				m_FirstMouse = true;
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
		}
		cPressedLastFrame = cPressedThisFrame;

		if (m_MouseCaptured)
		{
			float velocity = m_CameraTranslationSpeed * ts;

			if (Input::KeyPressed(MONSI_KEY_W))
				m_CameraPosition += m_CameraFront * velocity;

			if (Input::KeyPressed(MONSI_KEY_S))
				m_CameraPosition -= m_CameraFront * velocity;

			if (Input::KeyPressed(MONSI_KEY_A))
				m_CameraPosition -= m_CameraRight * velocity;

			if (Input::KeyPressed(MONSI_KEY_D))
				m_CameraPosition += m_CameraRight * velocity;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetDirection(m_CameraFront, m_CameraUp);
	}

	void PerspectiveControl::OnLayerEvent(Event& event)
	{
		ENGINE_PROFILER_FUNCTION();
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseEventMoved>(ENGINE_BIND_EVENT_FN(PerspectiveControl::OnMouseMoved));
		dispatcher.Dispatch<MouseEventScrolled>(ENGINE_BIND_EVENT_FN(PerspectiveControl::OnZoomEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(PerspectiveControl::OnResizeEvent));
	}

	void PerspectiveControl::OnWindowResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void PerspectiveControl::UpdateCameraVectors()
	{
		glm::vec3 front;

		if (m_Pitch > 89.9f)  m_Pitch = 89.9f;
		if (m_Pitch < -89.9f) m_Pitch = -89.9f;

		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(front);

		glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };

		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, worldUp));
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
	}

	bool PerspectiveControl::OnZoomEvent(MouseEventScrolled& event)
	{
		m_FOV -= event.GetYOffset();

		if (m_FOV < 1.0f)
			m_FOV = 1.0f;
		if (m_FOV > 90.0f)
			m_FOV = 90.0f;

		m_Camera.SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);

		return false;
	}

	bool PerspectiveControl::OnResizeEvent(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();

		m_Camera.SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);

		return false;
	}

	bool PerspectiveControl::OnMouseMoved(MouseEventMoved& event)
	{
		if (!m_MouseCaptured)
		{
			m_LastMouseX = event.GetMouseX();
			m_LastMouseY = event.GetMouseY();
			return false;
		}

		if (m_FirstMouse)
		{
			m_LastMouseX = event.GetMouseX();
			m_LastMouseY = event.GetMouseY();
			m_FirstMouse = false;
		}

		float xoffset = event.GetMouseX() - m_LastMouseX;
		float yoffset = m_LastMouseY - event.GetMouseY();

		m_LastMouseX = event.GetMouseX();
		m_LastMouseY = event.GetMouseY();

		xoffset *= m_MouseSensitivity;
		yoffset *= m_MouseSensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		UpdateCameraVectors();

		return false;
	}

}