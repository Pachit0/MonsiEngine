#include "Sandbox3D.h"
#include <imgui.h>
#include <imgui_internal.h>

ExampleLayer::ExampleLayer() : Layer("Sandbox3D"), m_CameraControl(1280.0f / 720.0f), m_ViewportSize{ 0.0f,0.0f }, m_ViewportFocused(false)
{

}

void ExampleLayer::OnLayerAttach()
{
	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);
	m_MonsiTest = Monsi::Texture2D::Create( TEXTURE_PATH "background.png");
}

void ExampleLayer::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_FrameBuffer->Bind();

	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_CameraControl.OnLayerUpdate(timestep);

// 	static float rotate = 0.0f;
// 	rotate += timestep * 50.0f;

	Monsi::Renderer3D::Begin3D(m_CameraControl);

	float spacing = 2.5f;
	glm::vec3 noRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				glm::vec3 position = glm::vec3(
					(x - 5.0f) * spacing,
					(y - 5.0f) * spacing,
					(z - 5.0f) * spacing
				);

				Monsi::Renderer3D::DrawCube(position, { 1.0f, 1.0f, 1.0f }, m_MonsiTest, noRotation);
			}
		}
	}

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

	ImGui::End();
}

void ExampleLayer::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}
