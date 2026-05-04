#include "MonsiPch.h"
#include "OrthographicControl.h"
#include "Input.h"
#include "MonsiKeyCodes.h"

namespace Monsi {

	OrthographicControl::OrthographicControl(float AspectRatio, bool cameraRotatoinFlag, bool cameraMovementFlag, bool cameraZoomFlag)
		: m_AspectRation(AspectRatio),
		m_Camera(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl),
		m_CameraRotationFlag(cameraRotatoinFlag),
		m_CameraMovementFlag(cameraMovementFlag),
		m_CameraZoomFlag(cameraZoomFlag)
	{

	}

	OrthographicControl::OrthographicControl(float AspectRatio, float Zoom, bool cameraRotatoinFlag, bool cameraMovementFlag, bool cameraZoomFlag)
		: m_AspectRation(AspectRatio), m_ZoomControl(Zoom),
		m_Camera(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl),
		m_CameraRotationFlag(cameraRotatoinFlag),
		m_CameraMovementFlag(cameraMovementFlag),
		m_CameraZoomFlag(cameraZoomFlag)
	{

	}

	OrthographicControl::OrthographicControl(float AspectRatio, float Zoom, const glm::vec2& cameraPosition, bool cameraRotatoinFlag, bool cameraMovementFlag, bool cameraZoomFlag)
		: m_AspectRation(AspectRatio), m_ZoomControl(Zoom),
		m_Camera(-m_AspectRation * m_ZoomControl, m_AspectRation* m_ZoomControl, -m_ZoomControl, m_ZoomControl),
		m_CameraPosition({ cameraPosition.x, cameraPosition.y, 0.0f }),
		m_CameraRotationFlag(cameraRotatoinFlag),
		m_CameraMovementFlag(cameraMovementFlag),
		m_CameraZoomFlag(cameraZoomFlag)
	{

	}

	void OrthographicControl::OnLayerUpdate(TimeStep timeStep) {
		ENGINE_PROFILER_FUNCTION();
		if(m_CameraMovementFlag){
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

	void OrthographicControl::OnWindowResize(float width, float height)
	{
		ENGINE_PROFILER_FUNCTION();
		m_AspectRation = width / height;
		m_Camera.SetProjection(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl);
	}

	bool OrthographicControl::OnZoomEvent(MouseEventScrolled& event)
	{
		ENGINE_PROFILER_FUNCTION();
		if (m_CameraZoomFlag) {
			m_ZoomControl -= event.GetYOffset() * 0.15f;

			if (m_ZoomControl < 0.25f) {
				m_ZoomControl = 0.25f;
			}

			m_Camera.SetProjection(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl);
			
			return true;
		}
		return false;
	}

	bool OrthographicControl::OnResizeEvent(WindowResizeEvent& event)
	{
		ENGINE_PROFILER_FUNCTION();
		OnWindowResize(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
		return false;
	}

	void OrthographicControl::SetZoom(float zoom)
	{
		m_ZoomControl = zoom;
		m_Camera.SetProjection(-m_AspectRation * m_ZoomControl, m_AspectRation * m_ZoomControl, -m_ZoomControl, m_ZoomControl);
	}
}