#include "MonsiPch.h"
#include "Engine.h"

#include "EventFormatter.h"
#include "Input.h"

#include <glad/glad.h>

namespace Monsi {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    
    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type)
        {
        case Monsi::ShaderDataType::Float:  return GL_FLOAT;
        case Monsi::ShaderDataType::Float2: return GL_FLOAT;
        case Monsi::ShaderDataType::Float3: return GL_FLOAT;   
        case Monsi::ShaderDataType::Float4: return GL_FLOAT;   
        case Monsi::ShaderDataType::Mat3:   return GL_FLOAT;
        case Monsi::ShaderDataType::Mat4:   return GL_FLOAT;
        case Monsi::ShaderDataType::Int:    return GL_INT;
        case Monsi::ShaderDataType::Int2:   return GL_INT;
        case Monsi::ShaderDataType::Int3:   return GL_INT;   
        case Monsi::ShaderDataType::Int4:   return GL_INT;   
        case Monsi::ShaderDataType::Bool:   return GL_BOOL;
        }
    }

    Application::Application() {
        ENGINE_ASSERT(!s_Instance, "Application object already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFn(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        float vertices[]{
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.3f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = { //  Vertex code needs the same layout order as here
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };

        m_VertexBuffer->SetLayout(layout);

        uint32_t index = 0;
        for (const auto& element : layout) {

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, 
                layout.GetStride(), (const void*)element.Offset);
            index++;
        }


        uint32_t indices[]{ 0 ,1 ,2 };

        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        m_Shader.reset(Shader::Create("D:/Monsi Engine/Engine/Renderer/vertexShader.vert", "D:/Monsi Engine/Engine/Renderer/fragmentShader.frag"));
    }

    Application::~Application() {

    }

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        ENGINE_LOG_TRACE("{0}", event);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
            (*--it)->OnLayerEvent(event);
            if (event.Handled) {
                break;
            }
        }
    }

    void Application::Run() {
        while (m_Running) {
            glClearColor(0.5, 0, 0.05, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack) {
                layer->OnLayerUpdate();
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

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnLayerAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnLayerAttach();
    }


}
