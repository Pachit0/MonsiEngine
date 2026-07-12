#include "Sandbox3D.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_CameraControl(1280.0f / 720.0f), m_ViewportSize{ 0.0f,0.0f },
m_ViewportFocused(false), m_ViewportHovered(false), m_SpherePosition({ -5.0f, 3.0f, 5.0f })
{
}

void Sandbox3D::OnLayerAttach()
{
	m_Scene = Monsi::CreateReference<Monsi::Scene>();
	m_CameraControl.SetCameraSpeed(50.0f);

	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);
	m_MonsiTest = Monsi::Texture2D::Create(TEXTURE_PATH "background.png");

	m_Backpack = Monsi::CreateReference<Monsi::Model>(MODEL_PATH "backpack/backpack.obj");

	Monsi::ModelImportSettings gamer;
	gamer.FlipUVs = false;
	m_Sponza = Monsi::CreateReference<Monsi::Model>(MODEL_PATH "crytek_sponza/sponza.obj", gamer);

	std::array<std::string, 6> skyboxTextures = {
		TEXTURE_PATH "right.png",
		TEXTURE_PATH "left.png",
		TEXTURE_PATH "top.png",
		TEXTURE_PATH "bottom.png",
		TEXTURE_PATH "front.png",
		TEXTURE_PATH "back.png"
	};
	m_SkyBoxTest = Monsi::CubeMapTexture::Create(skyboxTextures);

	m_ShpereMaterial = Monsi::CreateReference<Monsi::Material>();
	m_ShpereMaterial->AmbientColor = glm::vec3(0.247f, 0.199f, 0.074f);
	m_ShpereMaterial->DiffuseColor = glm::vec3(0.751f, 0.606f, 0.226f);
	m_ShpereMaterial->SpecularColor = glm::vec3(0.628f, 0.555f, 0.366f);
	m_ShpereMaterial->Shininess = 51.2f;

	m_SphereTest = Monsi::MeshBuilder::CreateSphere(1.0f, 32, 32, m_ShpereMaterial);
	m_TorusTest = Monsi::MeshBuilder::CreateTorus(2.5f, 0.5f, 32, 16, m_ShpereMaterial);
	m_ConeTest = Monsi::MeshBuilder::CreateCone(2.5f, 8, 32, m_ShpereMaterial);
	m_CylinderTest = Monsi::MeshBuilder::CreateCylinder(2.5f, 8, 32, m_ShpereMaterial);
	m_QuadTest = Monsi::MeshBuilder::CreateQuad(0.5f, 0.5f, m_ShpereMaterial);
	m_CubeTest = Monsi::MeshBuilder::CreateCube(0.5f, m_ShpereMaterial);

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComponent = m_CameraEntity.AddComponent<Monsi::CameraComponent>();
	cameraComponent.Camera.SetPerspective(glm::radians(45.0f), 0.1f, 1000.0f);
	cameraComponent.Primary = true;

	m_MainLightEntity = m_Scene->CreateEntity("Directional Light");
	auto& mainLight = m_MainLightEntity.AddComponent<Monsi::LightComponent>();
	mainLight.Type = Monsi::LightComponent::LightType::Directional;
	mainLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	mainLight.Color = glm::vec3(1.0f, 0.95f, 0.9f);
	mainLight.Intensity = 1.0f;

	m_PointLightEntity = m_Scene->CreateEntity("Point Light");
	auto& pointLight = m_PointLightEntity.AddComponent<Monsi::LightComponent>();
	pointLight.Type = Monsi::LightComponent::LightType::Point;
	pointLight.Color = glm::vec3(1.0f);
	pointLight.Intensity = 1.0f;
	pointLight.Radius = 10.0f;

	m_BackpackEntity = m_Scene->CreateEntity("Backpack");
	m_BackpackEntity.AddComponent<Monsi::ModelComponent>(m_Backpack);
	m_BackpackEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(-50.0f, 3.0f, 0.0f);

	m_SponzaEntity = m_Scene->CreateEntity("Sponza");
	m_SponzaEntity.AddComponent<Monsi::ModelComponent>(m_Sponza);
	m_SponzaEntity.GetComponent<Monsi::TransformComponent>().Scale = glm::vec3(0.05f);

	m_SphereEntity = m_Scene->CreateEntity("Sphere");
	m_SphereEntity.AddComponent<Monsi::MeshComponent>(m_SphereTest);
	m_SphereEntity.GetComponent<Monsi::TransformComponent>().Translation = m_SpherePosition;

	m_CubeEntity = m_Scene->CreateEntity("Cube");
	m_CubeEntity.AddComponent<Monsi::MeshComponent>(m_CubeTest);
	m_CubeEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(0.0f, 5.0f, 0.0f);

	m_TorusEntity = m_Scene->CreateEntity("Torus");
	m_TorusEntity.AddComponent<Monsi::MeshComponent>(m_TorusTest);
	m_TorusEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(0.0f, 1.0f, 0.0f);

	m_CylinderEntity = m_Scene->CreateEntity("Cylinder");
	m_CylinderEntity.AddComponent<Monsi::MeshComponent>(m_CylinderTest);
	m_CylinderEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(10.0f, 5.0f, 0.0f);

	m_ConeEntity = m_Scene->CreateEntity("Cone");
	m_ConeEntity.AddComponent<Monsi::MeshComponent>(m_ConeTest);
	m_ConeEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(20.0f, 5.0f, 0.0f);

	m_QuadEntity = m_Scene->CreateEntity("Quad");
	m_QuadEntity.AddComponent<Monsi::MeshComponent>(m_QuadTest);
	m_QuadEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(30.0f, 1.0f, 0.0f);

	m_Unit.SetContext(m_Scene);
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
		m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	m_FrameBuffer->Bind();

	Monsi::RenderCommand::SetClearColor({ 0.5f, 0.0f, 0.05f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_CameraControl.OnLayerUpdate(timestep);

	auto& camTransform = m_CameraEntity.GetComponent<Monsi::TransformComponent>();
	camTransform.Translation = m_CameraControl.GetCamera().GetPosition();
	camTransform.Rotation = glm::quatLookAt(m_CameraControl.GetCamera().GetFront(), m_CameraControl.GetCamera().GetUp());

	static float rotationStep = 0.0f;
	rotationStep += timestep * 50.0f;
	if (rotationStep >= 360.0f) rotationStep -= 360.0f;

	static float totalTime = 0.0f;
	totalTime += timestep;
	glm::vec3 animatedSpherePos = m_SpherePosition;
	animatedSpherePos.y += std::sin(totalTime * 2.0f) * 0.5f;

	auto& backpackTransform = m_BackpackEntity.GetComponent<Monsi::TransformComponent>();
	backpackTransform.Rotation = glm::angleAxis(glm::radians(rotationStep), glm::vec3(0.0f, 1.0f, 0.0f));

	m_SphereEntity.GetComponent<Monsi::TransformComponent>().Translation = animatedSpherePos;

	m_Scene->OnUpdate(timestep);

	Monsi::Renderer3D::DrawSkyBox(m_CameraControl.GetCamera().GetViewMatrix(), m_CameraControl.GetCamera().GetProjectionMatrix(), m_SkyBoxTest);

	m_FrameBuffer->Unbind();
}

void Sandbox3D::OnLayerDetach()
{
}

void Sandbox3D::OnImGuiDraw() {

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit")) {
			Monsi::Application::Get().CloseApp();
		}

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

	ImGui::Begin("Info");

	ImGui::Text("Renderer3D stats:");
	ImGui::Text("FPS: %.1f", m_FPS);
	ImGui::Text("Frame Time: %.3f ms", (1.0f / m_FPS) * 1000.0f);

	ImGui::Separator();

	auto& mainLight = m_MainLightEntity.GetComponent<Monsi::LightComponent>();
	ImGui::Text("Scene light parameters");
	ImGui::SliderFloat("Scene light Intensity", &mainLight.Intensity, 0.0f, 3.0f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(mainLight.Color));
	ImGui::SliderFloat3("Direction", glm::value_ptr(mainLight.Direction), -1.5f, 1.5f);

	ImGui::Separator();

	auto& pointLight = m_PointLightEntity.GetComponent<Monsi::LightComponent>();
	auto& pointLightTransform = m_PointLightEntity.GetComponent<Monsi::TransformComponent>();
	glm::vec3 pointLightPos = glm::vec3(pointLightTransform.Translation);

	ImGui::Text("Point light parameters");

	if (ImGui::SliderFloat3("Position", glm::value_ptr(pointLightPos), -10.0f, 10.0f))
		pointLightTransform.Translation = pointLightPos;

	ImGui::ColorEdit3("Color", glm::value_ptr(pointLight.Color));
	ImGui::SliderFloat("Point light Intensity", &pointLight.Intensity, 0.0f, 10.0f);
	ImGui::SliderFloat("Radius", &pointLight.Radius, 0.0f, 100.0f);

	ImGui::Separator();

	ImGui::Text("Sphere material parameters");
	ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ShpereMaterial->AmbientColor));
	ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_ShpereMaterial->DiffuseColor));
	ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_ShpereMaterial->SpecularColor));
	ImGui::SliderFloat("Shininess", &m_ShpereMaterial->Shininess, 1.0f, 64.0f);

	ImGui::End();

	m_Unit.OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });
	ImGui::Begin("Viewport");
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Monsi::Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };


	uint32_t textureID = m_FrameBuffer->GetColorAttachmentID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Scene");
	ImGui::Text("Monsi example scene");
	ImGui::Text("Press C when focused on the viewport\nwindow to move the camera with WASD");
	ImGui::End();
}

void Sandbox3D::OnLayerEvent(Monsi::Event& event)
{
	m_CameraControl.OnLayerEvent(event);
}