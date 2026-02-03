#include "MonsiPch.h"
#include "Engine.h"

#include "EventFormatter.h"
#include "Input.h"

#include "Renderer.h"
#include <glfw/glfw3.h>

namespace Monsi {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        ENGINE_ASSERT(!s_Instance, "Application object already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFn(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }

    Application::~Application() {

    }

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

        //ENGINE_LOG_TRACE("{0}", event);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
            (*--it)->OnLayerEvent(event);
            if (event.Handled) {
                break;
            }
        }
    }

    void Application::Run() {
        while (m_Running) {
            float currentTime = static_cast<float>(glfwGetTime());
            TimeStep timeStep = currentTime - m_PrevFrameTime;
            m_PrevFrameTime = currentTime;

            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack) {
                    layer->OnLayerUpdate(timeStep);
                }
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiDraw();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        m_Running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& event) {
        if (event.GetWidth() == 0 || event.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        Renderer::onWindowResize(event.GetWidth(), event.GetHeight());

        return false;
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnLayerAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnLayerAttach();
    }


}
