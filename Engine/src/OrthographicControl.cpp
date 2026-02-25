#include "MonsiPch.h"
#include "OrthographicControl.h"
#include "Input.h"
#include "MonsiKeyCodes.h"

namespace Monsi {



	OrthographicControl::OrthographicControl(float AspectRatio, bool cameraRotatoinFlag) 
		: m_AspectRation(AspectRatio), m_Camera(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl), m_CameraRotationFlag(cameraRotatoinFlag) {

	}

	void OrthographicControl::OnLayerUpdate(TimeStep timeStep) {
		ENGINE_PROFILER_FUNCTION();
		if (Input::KeyPressed(MONSI_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * timeStep;
		}
		else if (Input::KeyPressed(MONSI_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * timeStep;
		}
		if (Input::KeyPressed(MONSI_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * timeStep;
		}
		else if (Input::KeyPressed(MONSI_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * timeStep;
		}

		if (m_CameraRotationFlag) {
			if (Input::KeyPressed(MONSI_KEY_E)) {
				m_CameraRotation += m_CameraRotationSpeed * timeStep;
			}
			else if (Input::KeyPressed(MONSI_KEY_Q)) {
				m_CameraRotation -= m_CameraRotationSpeed * timeStep;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicControl::OnLayerEvent(Event& event) {
		ENGINE_PROFILER_FUNCTION();
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseEventScrolled>(ENGINE_BIND_EVENT_FN(OrthographicControl::OnZoomEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OrthographicControl::OnResizeEvent));
	}

	bool OrthographicControl::OnZoomEvent(MouseEventScrolled& event)
	{
		ENGINE_PROFILER_FUNCTION();
		m_ZoomControl -= event.GetYOffset() * 0.15f;
		if (m_ZoomControl < 0.25f) {
			m_ZoomControl = 0.25f;
		}
		m_Camera.SetProjection(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl);
		return false;
	}

	bool OrthographicControl::OnResizeEvent(WindowResizeEvent& event)
	{
		ENGINE_PROFILER_FUNCTION();
		m_AspectRation = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());
		m_Camera.SetProjection(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl);
		return false;
	}

}