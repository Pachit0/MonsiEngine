#pragma once

#include "Core.h"
#include "Events.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/VertexArray.h"
#include "Core/TimeStep.h"

#include "Window.h"

namespace Monsi {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool onWindowResize(WindowResizeEvent& event);

        Reference<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;

        LayerStack m_LayerStack;

        static Application* s_Instance;
        float m_PrevFrameTime;
    };

    Application* CreateApplication();

}