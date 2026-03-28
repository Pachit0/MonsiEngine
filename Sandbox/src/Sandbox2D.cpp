#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"
#include "debug/instrumentor.h"

//#include "thirdparty/implot/implot.h"
// struct RollingBuffer {
// 	float Span;
// 	ImVector<ImVec2> Data;
// 	RollingBuffer() {
// 		Span = 10.0f;
// 		Data.reserve(2000);
// 	}
// 	void AddPoint(float x, float y) {
// 		float xmod = fmodf(x, Span);
// 		if (!Data.empty() && xmod < Data.back().x)
// 			Data.shrink(0);
// 		Data.push_back(ImVec2(xmod, y));
// 	}
// };

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
	//ENGINE_PROFILER_SCOPE_LAMBDA("Sandbox2D::OnLayerUpdate1");
	ENGINE_PROFILER_FUNCTION();

	m_CameraControl.OnLayerUpdate(timestep);
	//ENGINE_PROFILER_SCOPE_LAMBDA("RenderCommand1");

	Monsi::Renderer2D::ResetBatchStatistics();
	{
		ENGINE_PROFILER_SCOPE("RenderCommand");
		Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
		Monsi::RenderCommand::Clear();
	}
	//ENGINE_PROFILER_SCOPE_LAMBDA("Begin2D1");
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
		//Monsi::Renderer2D::drawQuad({ -1.0f,0.0f }, { 0.8f, 0.8f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		//Monsi::Renderer2D::drawQuadRotated({ 0.5f,-0.5f}, { 0.5f, 0.75f }, { 1.0f, 0.0f, 1.0f, 1.0f }, rotate);
		//Monsi::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_MonsiTest);


		Monsi::Renderer2D::EndScene2D();
	}
}

void Sandbox2D::OnLayerDetach() {
	ENGINE_PROFILER_FUNCTION();
}

void Sandbox2D::OnImGuiDraw() {
	ENGINE_PROFILER_FUNCTION();
	ImGui::Begin("ImGui");

	auto batchStats = Monsi::Renderer2D::GetBatchStatistics();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", batchStats.DrawCalls);
	ImGui::Text("Quad Total Count: %d", batchStats.QuadCount);
	ImGui::Text("Quad Vertices: %d", batchStats.GetVertexCount());
	ImGui::Text("Quad Indices: %d", batchStats.GetIndexCount());
	
// 	for (auto& result : m_TimeResults) {
// 		char dest[64];
// 		strcpy(dest, result.Name);
// 		strcat(dest, " <- %.3f ms");
// 
// 		ImGui::Text(dest, result.Time);
// 	}
// 	ImPlot::CreateContext();
// 	static RollingBuffer   rdata1, rdata2, rdata3;
// 
// 	static float t = 0, last_t = 0.0f;
// 	if (t == 0 || t - last_t >= 0.02f) {
// 		rdata1.AddPoint(t, m_TimeResults[0].Time);
// 		rdata2.AddPoint(t, m_TimeResults[1].Time);
// 		rdata3.AddPoint(t, m_TimeResults[2].Time);
// 		last_t = t;
// 	}
// 	t += ImGui::GetIO().DeltaTime;
// 
// 	static float history = 10.0f;
// 	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
// 	rdata1.Span = history;
// 	rdata2.Span = history;
// 	rdata3.Span = history;
// 
// 	static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
// 
// 	if (ImPlot::BeginPlot("##Rolling", ImVec2(-1, ImGui::GetTextLineHeight() * 20))) {
// 		ImPlot::PushStyleVar(ImPlotStyleVar_MinorTickSize, {30.0f, 30.0f});
// 		ImPlot::PushStyleVar(ImPlotStyleVar_MajorTickSize, {30.0f, 30.0f});
// 
// 		ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
// 		ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
// 		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
// 		ImPlotSpec spec;
// 		spec.Offset = 0;
// 		spec.Stride = 2 * sizeof(float);
// 		ImPlot::PlotLine(m_TimeResults[0].Name, &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), spec);
// 		ImPlot::PlotLine(m_TimeResults[1].Name, &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), spec);
// 		ImPlot::PlotLine(m_TimeResults[2].Name, &rdata3.Data[0].x, &rdata3.Data[0].y, rdata3.Data.size(), spec);
// 		ImPlot::EndPlot();
// 	}

	ImGui::End();

	m_TimeResults.clear();
}

void Sandbox2D::OnLayerEvent(Monsi::Event& event){
	m_CameraControl.OnLayerEvent(event);
}
