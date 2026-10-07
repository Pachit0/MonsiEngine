#include "GameLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer()
	: Layer("GameLayer")
{
}

void GameLayer::OnLayerAttach()
{
	m_Scene = Monsi::CreateReference<Monsi::Scene>();

	// Setup Offscreen FrameBuffer
	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);

	// Primary Perspective Camera
	m_CameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComp = m_CameraEntity.AddComponent<Monsi::CameraComponent>();
	cameraComp.Camera.SetOrthographic(20, -1000.0f, 1000.0f);
	cameraComp.Primary = true;

	// Attach native camera controls (allows WASD + Mouse controls)
	m_CameraEntity.AddComponent<Monsi::NativeScriptComponent>().Bind<Monsi::PerspectiveCameraControllerScript>();

	// Position camera away from the origin so it can view objects at (0, 0, 0)
	auto& cameraTransform = m_CameraEntity.GetComponent<Monsi::TransformComponent>();
	cameraTransform.Translation = glm::vec3(0.0f, 2.0f, 5.0f);

	// Directional Light
	m_DirectionalLightEntity = m_Scene->CreateEntity("Directional Light", false);
	auto& lightComp = m_DirectionalLightEntity.AddComponent<Monsi::DirectionalLightComponent>();
	lightComp.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lightComp.Color = MonsiColors::White;
	lightComp.Intensity = 1.0f;

	Monsi::CubeParams cubeParams = { 1.0f };
	auto defaultMaterial = Monsi::CreateReference<Monsi::Material>();
	defaultMaterial->AmbientColor = glm::vec3(0.2f);
	defaultMaterial->DiffuseColor = glm::vec3(0.8f, 0.2f, 0.2f);
	defaultMaterial->SpecularColor = glm::vec3(1.0f);

	auto cubeMesh = Monsi::MeshBuilder::Create(cubeParams, defaultMaterial);

	m_CubeEntity = m_Scene->CreateEntity("Cube");
	m_CubeEntity.AddComponent<Monsi::StaticMeshComponent>(cubeMesh);
	m_CubeEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameLayer::OnLayerDetach()
{
}

void GameLayer::OnLayerUpdate(Monsi::TimeStep timestep)
{
	// Handle viewport resize dynamically
	Monsi::FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_Scene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}

	// Render Scene into FrameBuffer
	m_FrameBuffer->Bind();
	Monsi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.12f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_Scene->OnUpdate(timestep);

	m_FrameBuffer->Unbind();
	m_FrameBuffer->BlitToWindow();
}

void GameLayer::OnImGuiDraw()
{
	// Dockspace Setup
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiID dockspaceId = ImGui::GetID("BaseEngineDockspace");
	ImGui::DockSpaceOverViewport(dockspaceId, viewport, ImGuiDockNodeFlags_None);

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				Monsi::Application::Get().CloseApp();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Lock viewport window by including ImGuiWindowFlags_NoMove along with NoDecoration
	ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Viewport", nullptr, viewportFlags);

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Monsi::Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(textureID)), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

void GameLayer::OnLayerEvent(Monsi::Event& event)
{
	Monsi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Monsi::KeyEventPressed>(ENGINE_BIND_EVENT_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnKeyPressed(Monsi::KeyEventPressed& event)
{
	if (event.getRepeat())
		return false;

	return false;
}