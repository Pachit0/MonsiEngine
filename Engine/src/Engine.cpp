#include "MonsiPch.h"
#include "Engine.h"

#include "EventFormatter.h"
#include "Input.h"

#include <glad/glad.h>

namespace Monsi {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        ENGINE_ASSERT(!s_Instance, "Application object already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFn(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.3f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
        };

		std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = { //  Vertex code needs the same layout order as here
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float4, "aColor"}
        };

        // Set the layout first then add vertexBuffer!!!!
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[] = { 0 ,1 ,2 };

		std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVertexArray.reset(VertexArray::Create());
        
        float squareVertices[] = {
			-0.8f, -0.8f, 0.0f,
			0.8f, -0.8f, 0.0f,
			0.8f, 0.8f, 0.0f,
            -0.8f,0.8f, 0.0f
        };
        
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        BufferLayout squareLayout = {
            {ShaderDataType::Float3, "aPos"}
        };
        squareVB->SetLayout(squareLayout);
        m_SquareVertexArray->AddVertexBuffer(squareVB);
       
        uint32_t squareIndices[] = { 0,1,2,2,3,0 };

        std::shared_ptr<IndexBuffer> squareEB;
        squareEB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareEB);
        
        m_SquareShader.reset(Shader::Create("D:/Monsi Engine/Engine/Renderer/vertexShaderSquare.vert", "D:/Monsi Engine/Engine/Renderer/fragmentShaderSquare.frag"));
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

            m_SquareShader->Bind();
            m_SquareVertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_SquareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            m_Shader->Bind();
            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
