#include "Sandbox3D.h"
#include <imgui.h>
#include <imgui_internal.h>

ExampleLayer::ExampleLayer() : Layer("Sandbox3D"), m_CameraControl(1280.0f / 720.0f), m_ViewportSize{ 0.0f,0.0f }, m_ViewportFocused(false), m_ViewportHovered(false)
{

}

void ExampleLayer::OnLayerAttach()
{
	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);
	m_MonsiTest = Monsi::Texture2D::Create( TEXTURE_PATH "background.png");
	m_Model = Monsi::CreateReference<Monsi::Model>( MODEL_PATH "backpack/backpack.obj");
	m_ModelTwo = Monsi::CreateReference<Monsi::Model>( MODEL_PATH "wall_cabinet/wall_cabinet.obj");

	std::array<std::string, 6> skyboxTextures = {
		TEXTURE_PATH "right.png",
		TEXTURE_PATH "left.png",
		TEXTURE_PATH "top.png",
		TEXTURE_PATH "bottom.png",
		TEXTURE_PATH "front.png",
		TEXTURE_PATH "back.png"
	};

	m_SkyBoxTest = Monsi::CubeMapTexture::Create(skyboxTextures);
	
	currentFrameLighting.MainLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	currentFrameLighting.MainLight.Color = glm::vec3(1.0f, 0.95f, 0.9f);
	currentFrameLighting.MainLight.Intensity = 1.0f;
}

void ExampleLayer::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_FrameBuffer->Bind();

	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_CameraControl.OnLayerUpdate(timestep);

	Monsi::Renderer3D::SetLighting(currentFrameLighting);

	Monsi::Renderer3D::AddPointLight({ 0.5f, 0.5f, 0.5f }, { 1.0f,1.0f,1.0f }, 1.0f, 100.0f);

	Monsi::Renderer3D::Begin3D(m_CameraControl);

	static float rotationStep = 0;
	rotationStep += timestep * 50.0f;

	if (rotationStep >= 360.0f) {
		rotationStep -= 360.0f;
	}

	glm::vec3 rotation = { 0.0f, -90.0f,0.0f };
	glm::vec3 lightVisualizerPos = -currentFrameLighting.MainLight.Direction * 10.0f;
	Monsi::Renderer3D::DrawCube({0.0f,0.0f,-5.0f}, {1.0f, 1.0f, 1.0f}, m_MonsiTest, rotation);
	Monsi::Renderer3D::DrawCube(lightVisualizerPos, { 0.3f, 0.3f, 0.3f }, glm::vec4(currentFrameLighting.MainLight.Color, 1.0f), { 0.0f, 0.0f, 0.0f });

	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < 5; z++) {
				Monsi::Renderer3D::DrawModel(m_Model, { x + 5,y + 5, z + 5 }, { 0.5f, 0.5f, 0.5f }, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,rotationStep,0.0f });
			}
		}
	}
	Monsi::Renderer3D::DrawModel(m_ModelTwo, {1.5f,0.0f,-5.0f}, {0.5f, 0.5f, 0.5f}, {1.0f,1.0f,1.0f,1.0f}, rotation);
	Monsi::Renderer3D::DrawSkyBox(m_CameraControl.GetCamera().GetViewMatrix(), m_CameraControl.GetCamera().GetProjectionMatrix(), m_SkyBoxTest);

	Monsi::Renderer3D::End3D();
	
	m_FrameBuffer->Unbind();
}

void ExampleLayer::OnLayerDetach()
{

}

void ExampleLayer::OnImGuiDraw() {

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit")) { Monsi::Application::Get().CloseApp(); }
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_None);

	static bool first_time = true;
	if (first_time)
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		ImGuiID dock_id_main = dockspace_id;
		ImGuiID dock_id_left;

		ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);

		ImGuiID dock_id_left_top;
		ImGuiID dock_id_left_bottom;

		ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.5f, &dock_id_left_top, &dock_id_left_bottom);

		ImGui::DockBuilderDockWindow("Viewport", dock_id_main);
		ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
		ImGui::DockBuilderDockWindow("Scene", dock_id_left_bottom);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::Begin("Properties");

	auto batchStats = Monsi::Renderer2D::GetBatchStatistics();

	ImGui::Text("Renderer3D stats:");
	ImGui::Text("TODO");

	ImGui::SliderFloat("Intensity", &currentFrameLighting.MainLight.Intensity, 0.0f, 1.0f);
	ImGui::SliderFloat3("Color", (float*)&currentFrameLighting.MainLight.Color, 0.0f, 1.0f);
	ImGui::SliderFloat3("Direction", (float*)&currentFrameLighting.MainLight.Direction, -1.0f, 1.0f);

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });
	ImGui::Begin("Viewport");
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Monsi::Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 VpSize = ImGui::GetContentRegionAvail();

	if (m_ViewportSize != *(glm::vec2*)&VpSize) {
		m_ViewportSize = { VpSize.x, VpSize.y };
		m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));

		m_CameraControl.OnWindowResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Scene");

	ImGui::Text("Monsi example scene");
	ImGui::Text("Press C when focused on the viewport window to move the camera with WASD");

	ImGui::End();
}

void ExampleLayer::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}
