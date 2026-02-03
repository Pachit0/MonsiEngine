#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraControl(1280.0f / 720.0f) {}

void Sandbox2D::OnLayerAttach() {
	m_MonsiTest = Monsi::Texture2D::Create("D:/Monsi Engine/Sandbox/assets/Textures/background.png");
}

void Sandbox2D::OnLayerUpdate(Monsi::TimeStep timestep) {
	m_CameraControl.OnLayerUpdate(timestep);

	Monsi::RenderCommand::Clear();
	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });

	Monsi::Renderer2D::Begin2D(m_CameraControl.GetCamera());

	Monsi::Renderer2D::drawQuad({ -1.0f,0.0f }, { 0.8f, 0.8f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	Monsi::Renderer2D::drawQuad({ 0.5f,-0.5f }, { 0.5f, 0.75f }, { 1.0f, 1.0f, 0.0f, 1.0f });
	Monsi::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, {1.0f, 0.5f, 0.5f, 1.0f}, m_MonsiTest, 10.0f);

	Monsi::Renderer2D::End2D();
}

void Sandbox2D::OnLayerDetach() {

}

void Sandbox2D::OnImGuiDraw() {
	/*ImGui::Begin("Color picker");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();*/
}

void Sandbox2D::OnLayerEvent(Monsi::Event& event){
	m_CameraControl.OnLayerEvent(event);
}
