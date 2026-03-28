#include "Sandbox2D.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"
#include "debug/instrumentor.h"

static const char* s_MapTiles = "CGGGCGGGC"
								"GCGGGGGCG"
								"GGCGCGCGG"
								"GGGCGCGGG"
								"GGGGCGGGG";

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraControl(1280.0f / 720.0f, 7.0f) {}

void Sandbox2D::OnLayerAttach() {
	ENGINE_PROFILER_FUNCTION();
	m_MonsiTest = Monsi::Texture2D::Create("D:/Monsi Engine/Sandbox/assets/Textures/atlas_48x.png");
	m_Chair = Monsi::SubTexture2D::CreateSubTexture(m_MonsiTest, { 8, 8 }, { 48.0f, 48.0f });

	s_TextureMap['C'] = Monsi::SubTexture2D::CreateSubTexture(m_MonsiTest, { 0,8 }, { 48,48 });
	s_TextureMap['G'] = Monsi::SubTexture2D::CreateSubTexture(m_MonsiTest, { 4,8 }, { 48,48 });
}

void Sandbox2D::OnLayerUpdate(Monsi::TimeStep timestep) {

	ENGINE_PROFILER_FUNCTION();

	m_CameraControl.OnLayerUpdate(timestep);

	Monsi::Renderer2D::ResetBatchStatistics();
	{
		ENGINE_PROFILER_SCOPE("RenderCommand");
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
	}
}

void Sandbox2D::OnLayerDetach() {
	ENGINE_PROFILER_FUNCTION();
}

void Sandbox2D::OnImGuiDraw() {
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

	ImGui::DockSpaceOverViewport(dockspace_id,viewport,ImGuiDockNodeFlags_PassthruCentralNode);

	static bool first_time = true;
	if (first_time)
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		ImGuiID dock_id_main = dockspace_id;
		ImGuiID dock_id_left;

		ImGui::DockBuilderSplitNode(dock_id_main,ImGuiDir_Left,0.20f,&dock_id_left,&dock_id_main);

		ImGuiID dock_id_left_top;
		ImGuiID dock_id_left_bottom;

		ImGui::DockBuilderSplitNode(dock_id_left,ImGuiDir_Up,0.5f,&dock_id_left_top,&dock_id_left_bottom);

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
	uint32_t textureID = m_MonsiTest->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 2304.0f / 4, 768.0f / 4 });

	ImGui::End();

	ImGui::Begin("Scene");

	ImGui::Text("Monsi example scene");

	ImGui::End();
}

void Sandbox2D::OnLayerEvent(Monsi::Event& event){
	m_CameraControl.OnLayerEvent(event);
}
