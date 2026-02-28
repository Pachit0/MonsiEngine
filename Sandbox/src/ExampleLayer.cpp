#include "ExampleLayer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

ExampleLayer::ExampleLayer() 
	: Layer("ADD YOUR LAYER'S NAME HERE!"), m_CameraControl(1280.0f / 720.0f, true), m_SquareColor({ 0.8f, 0.1f, 0.1f }) {

	m_VertexArray = Monsi::VertexArray::Create();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.3f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
	};

	Monsi::Reference<Monsi::VertexBuffer> VBO = Monsi::VertexBuffer::Create(vertices, sizeof(vertices));

	Monsi::BufferLayout layout = { // Vertex code needs the same layout order as here
		{Monsi::ShaderDataType::Float3, "aPos"},
		{Monsi::ShaderDataType::Float4, "aColor"}
	};

	// Set the layout first then add vertexBuffer!!!!
	VBO->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(VBO);

	uint32_t indices[] = { 0 ,1 ,2 };

	Monsi::Reference<Monsi::IndexBuffer> EBO = Monsi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(EBO);

	m_SquareVertexArray = Monsi::VertexArray::Create();

	float squareVertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Monsi::Reference<Monsi::VertexBuffer> squareVB = Monsi::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	Monsi::BufferLayout squareLayout = {
		{Monsi::ShaderDataType::Float3, "aPos"},
		{Monsi::ShaderDataType::Float2, "aTexCoord"}
	};
	squareVB->SetLayout(squareLayout);
	m_SquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[] = { 0,1,2,2,3,0 };

	Monsi::Reference<Monsi::IndexBuffer> squareEB = Monsi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(squareEB);

	m_Shader = Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/Shader.glsl");
	m_SingleColorShader = Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/ShaderFlatColor.glsl");	
	auto textureShader = m_ShaderLibrary.Load("D:/Monsi Engine/Sandbox/assets/Shader/Textures.glsl");

	m_MonsiTest = Monsi::Texture2D::Create("D:/Monsi Engine/Sandbox/assets/Textures/MonsiTest.png");

	std::dynamic_pointer_cast<Monsi::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Monsi::OpenGLShader>(textureShader)->UploadInt("u_Texture", 0);

}

void ExampleLayer::OnLayerAttach() {

}

void ExampleLayer::OnLayerUpdate(Monsi::TimeStep timeStep) {

	m_CameraControl.OnLayerUpdate(timeStep);

	Monsi::RenderCommand::Clear();
	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });

	Monsi::Renderer::Begin(m_CameraControl.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	std::dynamic_pointer_cast<Monsi::OpenGLShader>(m_SingleColorShader)->Bind();
	std::dynamic_pointer_cast<Monsi::OpenGLShader>(m_SingleColorShader)->setVec3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 20; x++) {
			glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
			Monsi::Renderer::Sumbit(m_SquareVertexArray, m_SingleColorShader, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Textures");
	m_MonsiTest->Bind();
	Monsi::Renderer::Sumbit(m_SquareVertexArray, textureShader, 
		glm::translate(glm::mat4(1.0f),glm::vec3(0.75f,0.75f,0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Monsi::Renderer::End();
}

void ExampleLayer::OnLayerDetach() {

}

void ExampleLayer::OnLayerEvent(Monsi::Event& event) {
	m_CameraControl.OnLayerEvent(event);
	if (event.GetEventType() == Monsi::EventType::WindowResize) { (Monsi::WindowResizeEvent&)event; }
}



void ExampleLayer::OnImGuiDraw() {
	ImGui::Begin("Color picker");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}