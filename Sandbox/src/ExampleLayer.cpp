#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("Sandbox3D"), m_CameraControl(1280.0f / 720.0f)
{

}

void ExampleLayer::OnLayerAttach()
{
	m_MonsiTest = Monsi::Texture2D::Create( TEXTURE_PATH "background.png");
}

void ExampleLayer::OnLayerUpdate(Monsi::TimeStep timestep)
{
	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_CameraControl.OnLayerUpdate(timestep);
	
	static float rotate = 0.0f;
	rotate += timestep * 50.0f;

	Monsi::Renderer3D::Begin3D(m_CameraControl);
	Monsi::Renderer3D::DrawCube({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_MonsiTest);

	//Monsi::Renderer3D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1.0f, 1.0f }, { 1.0f,1.0f,1.0f,1.0f }, glm::vec3(0.0f));

	Monsi::Renderer3D::End3D();
}

void ExampleLayer::OnLayerDetach()
{

}

void ExampleLayer::OnImGuiDraw()
{
	ImGui::Begin("ImGui");

	ImGui::End();
}

void ExampleLayer::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}
