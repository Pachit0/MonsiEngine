#pragma once

#include "Components.h"
#include "ScriptableEntity.h"
#include "TimeStep.h"
#include "Input.h"
#include "MonsiKeyCodes.h"
#include "MonsiMouseButtonCodes.h"
#include "Engine.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <GLFW/glfw3.h>

namespace Monsi {

	class PerspectiveCameraControllerScript : public ScriptableEntity
	{
	public:
		enum class LookMode { RightClick = 0, FPS = 1 };

		void OnCreate()
		{
			auto& transform = GetComponent<TransformComponent>();
			transform.Rotation = glm::quat(glm::vec3(glm::radians(m_Pitch), glm::radians(m_Yaw), glm::radians(m_Roll)));
		}

		void OnDestroy()
		{
			if (m_MouseCaptured)
				SetMouseCaptured(false);
		}

		void OnUpdate(TimeStep ts)
		{
			auto& transform = GetComponent<TransformComponent>();

			if (m_LookMode == LookMode::FPS)
				HandleFPSToggle();

			glm::vec3 forward = glm::rotate(transform.Rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			glm::vec3 right = glm::rotate(transform.Rotation, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::vec3 up = glm::rotate(transform.Rotation, glm::vec3(0.0f, 1.0f, 0.0f));

			bool movementEnabled = (m_LookMode == LookMode::RightClick) || m_MouseCaptured;

			if (movementEnabled)
			{
				float velocity = m_TranslationSpeed * ts;

				if (Input::KeyPressed(MONSI_KEY_W))
					transform.Translation += forward * velocity;

				if (Input::KeyPressed(MONSI_KEY_S))
					transform.Translation -= forward * velocity;

				if (Input::KeyPressed(MONSI_KEY_A))
					transform.Translation -= right * velocity;

				if (Input::KeyPressed(MONSI_KEY_D))
					transform.Translation += right * velocity;

				if (Input::KeyPressed(MONSI_KEY_SPACE))
					transform.Translation += up * velocity;

				if (Input::KeyPressed(MONSI_KEY_LEFT_CONTROL))
					transform.Translation -= up * velocity;
			}

			if (m_LookMode == LookMode::FPS)
				UpdateLookFPS();
			else
				UpdateLookRightClick();

			transform.Rotation = glm::quat(glm::vec3(glm::radians(m_Pitch), glm::radians(m_Yaw), glm::radians(m_Roll)));
		}

		void SetTranslationSpeed(float speed) { m_TranslationSpeed = speed; }
		void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }

		void SetYawPitchRoll(float yaw, float pitch, float roll)
		{
			m_Yaw = yaw;
			m_Pitch = pitch;
			m_Roll = roll;
		}

		glm::vec3 GetYawPitchRoll() {
			return glm::vec3{ m_Yaw,m_Pitch,m_Roll };
		}

		LookMode GetLookMode() const { return m_LookMode; }

		void SetLookMode(LookMode mode)
		{
			if (mode == m_LookMode)
				return;

			if (m_LookMode == LookMode::FPS && m_MouseCaptured)
				SetMouseCaptured(false);

			m_WasLookingLastFrame = false;
			m_FirstMouse = true;

			const auto& mousePos = Input::GetMousePos();
			m_LastMouseX = mousePos.first;
			m_LastMouseY = mousePos.second;

			m_LookMode = mode;
		}

	private:
		void HandleFPSToggle()
		{
			bool cPressedThisFrame = Input::KeyPressed(MONSI_KEY_C);

			if (cPressedThisFrame && !m_CPressedLastFrame)
				SetMouseCaptured(!m_MouseCaptured);

			m_CPressedLastFrame = cPressedThisFrame;
		}

		void SetMouseCaptured(bool captured)
		{
			m_MouseCaptured = captured;

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

		void UpdateLookFPS()
		{
			if (!m_MouseCaptured)
				return;

			const auto& mousePos = Input::GetMousePos();

			if (m_FirstMouse)
			{
				m_LastMouseX = mousePos.first;
				m_LastMouseY = mousePos.second;
				m_FirstMouse = false;
				return;
			}

			float deltaX = mousePos.first - m_LastMouseX;
			float deltaY = m_LastMouseY - mousePos.second;

			m_LastMouseX = mousePos.first;
			m_LastMouseY = mousePos.second;

			deltaX = -deltaX;

			ApplyLookDelta(deltaX, deltaY);
		}

		void UpdateLookRightClick()
		{
			bool rightMouseDown = Input::MouseButtonPressed(MONSI_MOUSE_BUTTON_RIGHT);

			const auto& mousePos = Input::GetMousePos();

			if (!rightMouseDown)
			{
				m_LastMouseX = mousePos.first;
				m_LastMouseY = mousePos.second;
				m_WasLookingLastFrame = false;
				return;
			}

			if (!m_WasLookingLastFrame)
			{
				m_LastMouseX = mousePos.first;
				m_LastMouseY = mousePos.second;
				m_WasLookingLastFrame = true;
			}

			float deltaX = mousePos.first - m_LastMouseX;
			float deltaY = m_LastMouseY - mousePos.second;

			m_LastMouseX = mousePos.first;
			m_LastMouseY = mousePos.second;

			ApplyLookDelta(deltaX, -deltaY);
		}

		void ApplyLookDelta(float deltaX, float deltaY)
		{
			m_Yaw += deltaX * m_MouseSensitivity;
			m_Pitch += deltaY * m_MouseSensitivity;
		}

	private:
		LookMode m_LookMode = LookMode::FPS;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;

		float m_TranslationSpeed = 15.0f;
		float m_MouseSensitivity = 0.1f;

		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;

		bool  m_FirstMouse = true;
		bool  m_WasLookingLastFrame = false;
		bool  m_MouseCaptured = false;
		bool m_CPressedLastFrame = false;

	};

}