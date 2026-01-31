#include "ExampleLayer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

ExampleLayer::ExampleLayer() 
	: Layer("ADD YOUR LAYER'S NAME HERE!"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraSpeed(10.0f),
	m_CameraRotation(0.0f), m_CameraRotationSpeed(90.0f), m_SquareColor({ 0.8f, 0.1f, 0.1f }) {

	m_VertexArray.reset(Monsi::VertexArray::Create());

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.3f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
	};

	Monsi::Reference<Monsi::VertexBuffer> VBO;
	VBO.reset(Monsi::VertexBuffer::Create(vertices, sizeof(vertices)));

	Monsi::BufferLayout layout = { // Vertex code needs the same layout order as here
		{Monsi::ShaderDataType::Float3, "aPos"},
		{Monsi::ShaderDataType::Float4, "aColor"}
	};

	// Set the layout first then add vertexBuffer!!!!
	VBO->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(VBO);

	uint32_t indices[] = { 0 ,1 ,2 };

	Monsi::Reference<Monsi::IndexBuffer> EBO;
	EBO.reset(Monsi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(EBO);

	m_SquareVertexArray.reset(Monsi::VertexArray::Create());

	float squareVertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Monsi::Reference<Monsi::VertexBuffer> squareVB;
	squareVB.reset(Monsi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	Monsi::BufferLayout squareLayout = {
		{Monsi::ShaderDataType::Float3, "aPos"},
		{Monsi::ShaderDataType::Float2, "aTexCoord"}
	};
	squareVB->SetLayout(squareLayout);
	m_SquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[] = { 0,1,2,2,3,0 };

	Monsi::Reference<Monsi::IndexBuffer> squareEB;
	squareEB.reset(Monsi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(squareEB);

	m_Shader = Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/Shader.glsl");
	m_SingleColorShader = Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/ShaderFlatColor.glsl");	
	auto textureShader = m_ShaderLibrary.Load("D:/Monsi Engine/Sandbox/assets/Shader/Textures.glsl");

	m_MonsiTest = Monsi::Texture2D::Create("D:/Monsi Engine/Sandbox/assets/Textures/MonsiTest.png");

	std::dynamic_pointer_cast<Monsi::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Monsi::OpenGLShader>(textureShader)->setInt("u_Texture", 0);

}

void ExampleLayer::OnLayerAttach() {

}

void ExampleLayer::OnLayerUpdate(Monsi::TimeStep timeStep) {

	Monsi::RenderCommand::Clear();
	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });

	if (Monsi::Input::KeyPressed(MONSI_KEY_D)) {
		m_CameraPosition.x += m_CameraSpeed * timeStep;
	}
	else if (Monsi::Input::KeyPressed(MONSI_KEY_A)) {
		m_CameraPosition.x -= m_CameraSpeed * timeStep;
	}
	if (Monsi::Input::KeyPressed(MONSI_KEY_S)) {
		m_CameraPosition.y -= m_CameraSpeed * timeStep;
	}
	else if (Monsi::Input::KeyPressed(MONSI_KEY_W)) {
		m_CameraPosition.y += m_CameraSpeed * timeStep;
	}

	if (Monsi::Input::KeyPressed(MONSI_KEY_E)) {
		m_CameraRotation += m_CameraRotationSpeed * timeStep;
	}
	else if (Monsi::Input::KeyPressed(MONSI_KEY_Q)) {
		m_CameraRotation -= m_CameraRotationSpeed * timeStep;
	}

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(m_CameraRotation);

	Monsi::Renderer::Begin(m_Camera);

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

}



void ExampleLayer::OnImGuiDraw() {
	ImGui::Begin("Color picker");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}