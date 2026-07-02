#include "Sandbox3D.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_CameraControl(1280.0f / 720.0f), m_ViewportSize{ 0.0f,0.0f }, m_ViewportFocused(false), m_ViewportHovered(false)
	, m_SpherePosition({ -5.0f, 3.0f, 5.0f }), m_Intensity(1.0f), m_Radius(10.0f), m_LightPointPosition{0.0f,0.0f,0.0f}, m_LightPointColor(1.0f,1.0f,1.0f,1.0f)
{}

void Sandbox3D::OnLayerAttach()
{
	m_CameraControl.setCameraSpeed(50.0f);

	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);
	m_MonsiTest = Monsi::Texture2D::Create( TEXTURE_PATH "background.png");

	m_Backpack = Monsi::CreateReference<Monsi::Model>( MODEL_PATH "backpack/backpack.obj");

	Monsi::ModelImportSettings gamer;
	gamer.FlipUVs = false;
	m_Sponza = Monsi::CreateReference<Monsi::Model>( MODEL_PATH "crytek_sponza/sponza.obj", gamer);

	std::array<std::string, 6> skyboxTextures = {
		TEXTURE_PATH "right.png",
		TEXTURE_PATH "left.png",
		TEXTURE_PATH "top.png",
		TEXTURE_PATH "bottom.png",
		TEXTURE_PATH "front.png",
		TEXTURE_PATH "back.png"
	};

	m_SkyBoxTest = Monsi::CubeMapTexture::Create(skyboxTextures);

	m_SceneLighting.MainLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_SceneLighting.MainLight.Color = glm::vec3(1.0f, 0.95f, 0.9f);
	m_SceneLighting.MainLight.Intensity = 1.0f;

	Monsi::PointLight lightTest;
	lightTest.Color = m_LightPointColor;
	lightTest.Position = m_LightPointPosition;
	lightTest.Intensity = m_Intensity;
	lightTest.Radius = m_Radius;

	m_SceneLighting.PointLights.emplace_back(lightTest);

	m_ShpereMaterial = Monsi::CreateReference<Monsi::Material>();

	m_ShpereMaterial->AmbientColor = glm::vec4(0.247f, 0.199f, 0.074f, 1.0f);
	m_ShpereMaterial->DiffuseColor = glm::vec4(0.751f, 0.606f, 0.226f, 1.0f);
	m_ShpereMaterial->SpecularColor = glm::vec4(0.628f, 0.555f, 0.366f, 1.0f);
	m_ShpereMaterial->Shininess = 51.2f;

	m_SphereTest = Monsi::MeshBuilder::CreateSphere(1.0f, 32, 32, m_ShpereMaterial);

	m_SphereTest = Monsi::MeshBuilder::CreateSphere(1.0f, 32, 32, m_ShpereMaterial);
}

void Sandbox3D::OnLayerUpdate(Monsi::TimeStep timestep)
{
	m_FrameTimeAccumulator += timestep;
	m_FrameCount++;

	if (m_FrameTimeAccumulator >= m_UpdateInterval)
	{
		m_FPS = (float)m_FrameCount / m_FrameTimeAccumulator;
		m_FrameCount = 0;
		m_FrameTimeAccumulator = 0.0f;
	}

	if (Monsi::FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CameraControl.OnWindowResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	m_FrameBuffer->Bind();

	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_CameraControl.OnLayerUpdate(timestep);
	Monsi::Renderer3D::SetSceneLighting(m_SceneLighting);

	Monsi::Renderer3D::Begin3D(m_CameraControl);

	static float rotationStep = 0;
	rotationStep += timestep * 50.0f;

	if (rotationStep >= 360.0f) {
		rotationStep -= 360.0f;
	}

	static float totalTime = 0.0f;
	totalTime += timestep;

	glm::vec3 animatedSpherePos = m_SpherePosition;
	animatedSpherePos.y += std::sin(totalTime * 2.0f) * 0.5f;

	glm::vec3 rotation = { 0.0f, -90.0f,0.0f };

	Monsi::Renderer3D::DrawCube(m_SceneLighting.PointLights[0].Position, { 0.3f, 0.3f, 0.3f }, { m_SceneLighting.PointLights[0].Color, 1.0f}, { 0.0f, 0.0f, 0.0f });

	glm::vec3 lightVisualizerPos = -m_SceneLighting.MainLight.Direction * 10.0f;
	Monsi::Renderer3D::DrawCube(lightVisualizerPos, { 0.3f, 0.3f, 0.3f }, { m_SceneLighting.MainLight.Color, 1.0f}, { 0.0f, 0.0f, 0.0f });

	Monsi::Renderer3D::DrawModel(m_Backpack, { -50.0f, 3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,rotationStep,0.0f });
	Monsi::Renderer3D::DrawModel(m_Sponza, { 0.0f,0.0f,0.0f }, { 0.05f, 0.05f, 0.05f }, { 1.0f,1.0f,1.0f,1.0f });

	Monsi::Renderer3D::DrawMesh(m_SphereTest.get(), animatedSpherePos, { 1.0f, 1.0f, 1.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f, 0.0f, 0.0f });

	Monsi::Renderer3D::DrawSkyBox(m_CameraControl.GetCamera().GetViewMatrix(), m_CameraControl.GetCamera().GetProjectionMatrix(), m_SkyBoxTest);

	Monsi::Renderer3D::End3D();
	
	m_FrameBuffer->Unbind();
}

void Sandbox3D::OnLayerDetach()
{

}

void Sandbox3D::OnImGuiDraw() {

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

	ImGui::Text("FPS: %.1f", m_FPS);
	ImGui::Text("Frame Time: %.3f ms", (1.0f / m_FPS) * 1000.0f);

	ImGui::Separator();

	ImGui::Text("Scene light parameters");
	ImGui::SliderFloat("Scene light Intensity", &m_SceneLighting.MainLight.Intensity, 0.0f, 3.0f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(m_SceneLighting.MainLight.Color));
	ImGui::SliderFloat3("Direction", glm::value_ptr(m_SceneLighting.MainLight.Direction), -1.5f, 1.5f);

	ImGui::Separator();

	ImGui::Text("Point light parameters");
	ImGui::SliderFloat3("Position", glm::value_ptr(m_SceneLighting.PointLights[0].Position), -10.0f, 10.0f);
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SceneLighting.PointLights[0].Color));
	ImGui::SliderFloat("Point light Intensity", &m_SceneLighting.PointLights[0].Intensity, 0.0f, 10.0f);
	ImGui::SliderFloat("Radius", &m_SceneLighting.PointLights[0].Radius, 0.0f, 100.0f);

	ImGui::Text("Sphere material parameters");
	ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ShpereMaterial->AmbientColor));
	ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_ShpereMaterial->DiffuseColor));
	ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_ShpereMaterial->SpecularColor));
	ImGui::SliderFloat("Shininess", &m_ShpereMaterial->Shininess, 1.0f, 64.0f);

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
	ImGui::Text("Press C when focused on the \n viewport window to move the camera with WASD");

	ImGui::End();
}

void Sandbox3D::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}
