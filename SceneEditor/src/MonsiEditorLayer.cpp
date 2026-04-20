#include "MonsiEditorLayer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>
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

		s_TextureMap['C'] = Monsi::SubTexture2D::CreateSubTexture(m_MonsiTest, { 0,8 }, { 48,48 });
		s_TextureMap['G'] = Monsi::SubTexture2D::CreateSubTexture(m_MonsiTest, { 4,8 }, { 48,48 });

		Monsi::FrameBufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = Monsi::FrameBuffer::Create(spec);

	}

	void EditorLayer::OnLayerUpdate(Monsi::TimeStep timestep) {

		ENGINE_PROFILER_FUNCTION();

		if (m_ViewportFocused) {
			m_CameraControl.OnLayerUpdate(timestep);
		}

		Monsi::Renderer2D::ResetBatchStatistics();
		{
			ENGINE_PROFILER_SCOPE("RenderCommand");
			m_FrameBuffer->Bind();
			Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
			Monsi::RenderCommand::Clear();
		}

		{
			static float rotate = 0.0f;
			rotate += timestep * 100.0f;
			ENGINE_PROFILER_SCOPE("Begin2D");
			Monsi::Renderer2D::BeginScene2D(m_CameraControl.GetCamera());

			for (uint32_t y = 0; y < 5; y++) {
				for (uint32_t x = 0; x < 9; x++) {
					char tileType = s_MapTiles[x + y * 9];
					Monsi::Reference<Monsi::SubTexture2D> texture;
					if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
						texture = s_TextureMap[tileType];
					}
					else {
						texture = m_Chair;
					}
					Monsi::Renderer2D::drawQuad({ x - 5.0f, y - 2.5f }, { 1.0f, 1.0f }, texture);
				}
			}


			Monsi::Renderer2D::EndScene2D();
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnLayerDetach() {
		ENGINE_PROFILER_FUNCTION();
	}

	void EditorLayer::OnImGuiDraw() {
		ENGINE_PROFILER_FUNCTION();

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

	void EditorLayer::OnLayerEvent(Monsi::Event& event) {
		m_CameraControl.OnLayerEvent(event);
	}

}