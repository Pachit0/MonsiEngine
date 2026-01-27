#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("ADD YOUR LAYER'S NAME HERE!") {

	m_VertexArray.reset(Monsi::VertexArray::Create());

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.3f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
	};

	std::shared_ptr<Monsi::VertexBuffer> VBO;
	VBO.reset(Monsi::VertexBuffer::Create(vertices, sizeof(vertices)));

	Monsi::BufferLayout layout = { //  Vertex code needs the same layout order as here
		{Monsi::ShaderDataType::Float3, "aPos"},
		{Monsi::ShaderDataType::Float4, "aColor"}
	};

	// Set the layout first then add vertexBuffer!!!!
	VBO->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(VBO);

	uint32_t indices[] = { 0 ,1 ,2 };

	std::shared_ptr<Monsi::IndexBuffer> EBO;
	EBO.reset(Monsi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(EBO);

	m_SquareVertexArray.reset(Monsi::VertexArray::Create());

	float squareVertices[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,
		-0.8f,0.8f, 0.0f
	};

	std::shared_ptr<Monsi::VertexBuffer> squareVB;
	squareVB.reset(Monsi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	Monsi::BufferLayout squareLayout = {
		{Monsi::ShaderDataType::Float3, "aPos"}
	};
	squareVB->SetLayout(squareLayout);
	m_SquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[] = { 0,1,2,2,3,0 };

	std::shared_ptr<Monsi::IndexBuffer> squareEB;
	squareEB.reset(Monsi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(squareEB);

	m_SingleColorShader.reset(Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/vertexShaderSquare.vert", "D:/Monsi Engine/Sandbox/assets/Shader/fragmentShaderSquare.frag"));
	
	m_Shader.reset(Monsi::Shader::Create("D:/Monsi Engine/Sandbox/assets/Shader/vertexShader.vert", "D:/Monsi Engine/Sandbox/assets/Shader/fragmentShader.frag"));

}

void ExampleLayer::OnLayerAttach() {

}

void ExampleLayer::OnLayerUpdate() {
	Monsi::RenderCommand::Clear();
	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });

	Monsi::Renderer::Begin();

	m_SingleColorShader->Bind();
	Monsi::Renderer::Sumbit(m_SquareVertexArray);

	m_Shader->Bind();
	Monsi::Renderer::Sumbit(m_VertexArray);

	Monsi::Renderer::End();
}

void ExampleLayer::OnLayerDetach() {

}

void ExampleLayer::OnLayerEvent(Monsi::Event& event) {
	CLIENT_LOG_TRACE("{0}", event);
}

void ExampleLayer::OnImGuiDraw() {
	ImGui::Begin("TEST");
	ImGui::Text("Hello world");
	ImGui::End();
}