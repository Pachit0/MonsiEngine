#include "MonsiEditorLayer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <debug/instrumentor.h>

namespace Monsi {
	
	static const char* s_MapTiles = "CGGGCGGGC"
		"GCGGGGGCG"
		"GGCGCGCGG"
		"GGGCGCGGG"
		"GGGGCGGGG";

	EditorLayer::EditorLayer() 
		: Layer("EditorLayer"), m_CameraControl(1280.0f / 720.0f, 7.0f), m_ViewportSize{ 0.0f,0.0f }, m_ViewportFocused(false)
	{
	}

	void EditorLayer::OnLayerAttach() {
		ENGINE_PROFILER_FUNCTION();
		m_MonsiTest = Texture2D::Create( TEXTURE_PATH "atlas_48x.png");
		m_Chair = SubTexture2D::CreateSubTexture(m_MonsiTest, { 8, 8 }, { 48.0f, 48.0f });

		s_TextureMap['C'] = SubTexture2D::CreateSubTexture(m_MonsiTest, { 0,8 }, { 48,48 });
		s_TextureMap['G'] = SubTexture2D::CreateSubTexture(m_MonsiTest, { 4,8 }, { 48,48 });

		FrameBufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateReference<Scene>();

		auto entity = m_ActiveScene->CreateEntity();
		m_ActiveScene->Reg().emplace<TransformComponent>(entity);
		m_ActiveScene->Reg().emplace<SpriteRendererComponent>(entity, glm::vec4({ 0.0f,0.0f,1.0f,1.0f }));

	}

	void EditorLayer::OnLayerUpdate(TimeStep timestep) {

		ENGINE_PROFILER_FUNCTION();

		if (m_ViewportFocused) {
			m_CameraControl.OnLayerUpdate(timestep);
		}

		Renderer2D::ResetBatchStatistics();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
		RenderCommand::Clear();


		Renderer2D::BeginScene2D(m_CameraControl.GetCamera());

		m_ActiveScene->OnUpdate(timestep);

		Renderer2D::EndScene2D();

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnLayerDetach() {
		ENGINE_PROFILER_FUNCTION();
	}

	void EditorLayer::OnImGuiDraw() {
		ENGINE_PROFILER_FUNCTION();

		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { Application::Get().CloseApp(); }
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

		auto batchStats = Renderer2D::GetBatchStatistics();

		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw Calls: %d", batchStats.DrawCalls);
		ImGui::Text("Quad Total Count: %d", batchStats.QuadCount);
		ImGui::Text("Quad Vertices: %d", batchStats.GetVertexCount());
		ImGui::Text("Quad Indices: %d", batchStats.GetIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

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

	void EditorLayer::OnLayerEvent(Event& event) {
		m_CameraControl.OnLayerEvent(event);
	}

}