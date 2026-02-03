#pragma once

#include "OrthographicCamera.h"
#include "Core/TimeStep.h"
#include "Events.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Monsi {

	class OrthographicControl {

	public:
		OrthographicControl(float AspectRatio, bool cameraRotatoinFlag = false);

		void OnLayerUpdate(TimeStep timestep);
		void OnLayerEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoom(float zoom) { m_ZoomControl = zoom; }
		float GetZoom() const { return m_ZoomControl; }

	private:
		bool m_CameraRotationFlag;
		float m_ZoomControl = 1.0f;
		float m_AspectRation;
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 180.0f;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		OrthographicCamera m_Camera;

		bool OnZoomEvent(MouseEventScrolled& event);
		bool OnResizeEvent(WindowResizeEvent& event);
	};
	
}