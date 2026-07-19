#pragma once

#include "PerspectiveCamera.h"
#include "Core/TimeStep.h"
#include "Events.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Monsi {

    class PerspectiveControl
    {
    public:
        PerspectiveControl(float aspectRatio, bool rotationFlag = true);
        PerspectiveControl(const glm::vec3& position, float aspectRatio, bool rotationFlag = true);
        PerspectiveControl(const glm::vec3& position, const glm::vec3& direction, float aspectRatio, bool rotationFlag = true);

        void OnLayerUpdate(TimeStep timestep);
        void OnLayerEvent(Event& event);

        void OnWindowResize(float width, float height);

        PerspectiveCamera& GetCamera() { return m_Camera; }
        const PerspectiveCamera& GetCamera() const { return m_Camera; }
        bool IsMouseCaptured() const { return m_MouseCaptured; }

        float GetYaw() { return m_Yaw; }
        float GetPitch() { return m_Pitch; }
        void SetCameraSpeed(float speed) { m_CameraTranslationSpeed = speed; }
        void SetFOV(float fov) { m_FOV = fov; }
        float GetFOV() const { return m_FOV; }

    private:
		bool m_MouseCaptured = false;
		bool m_FirstMouse = true;
        bool m_CameraRotationFlag;

		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;
        float m_AspectRatio;
        float m_FOV = 45.0f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.0f;
        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        float m_CameraTranslationSpeed = 5.0f;
        float m_MouseSensitivity = 0.1f;

        glm::vec3 m_CameraPosition;
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraUp;
        glm::vec3 m_CameraRight;

        PerspectiveCamera m_Camera;

    private:
        bool OnZoomEvent(MouseEventScrolled& event);
        bool OnResizeEvent(WindowResizeEvent& event);
        bool OnMouseMoved(MouseEventMoved& event);

        void UpdateCameraVectors();
    };

}