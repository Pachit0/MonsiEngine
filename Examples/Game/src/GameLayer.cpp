#include "GameLayer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer()
	: Layer("GameLayer")
{
}

void GameLayer::OnLayerAttach()
{
	m_Scene = Monsi::CreateReference<Monsi::Scene>();

	Monsi::FrameBufferSpec spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Monsi::FrameBuffer::Create(spec);

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComp = m_CameraEntity.AddComponent<Monsi::CameraComponent>();
	cameraComp.Camera.SetOrthographic(25.0f, -1000.0f, 1000.0f);
	cameraComp.Primary = true;

	{
		auto& nativeScript = m_CameraEntity.AddComponent<Monsi::NativeScriptComponent>();
		nativeScript.Bind<Monsi::CameraFollowScript>();
	}

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

	m_CubeEntity = m_Scene->CreateEntity("KUKU");
	m_CubeEntity.AddComponent<Monsi::StaticMeshComponent>(cubeMesh);
	m_CubeEntity.GetComponent<Monsi::TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.0f);

	{
		auto& playerScript = m_CubeEntity.AddComponent<Monsi::NativeScriptComponent>();
		playerScript.Bind<Monsi::PlayerControllerScript>();
	}

	{
		auto& cameraScript = m_CameraEntity.GetComponent<Monsi::NativeScriptComponent>();
		cameraScript.Instance = cameraScript.InstantiateFuncPtr();
		cameraScript.Instance->Init(m_CameraEntity);

		if (auto* follow = cameraScript.GetScriptAs<Monsi::CameraFollowScript>())
		{
			follow->SetTarget(m_CubeEntity);
			follow->SetPositionOffset({ 0.0f, 3.0f, 6.0f });
			follow->SetRotationOffset(glm::quat(glm::radians(glm::vec3(-25.0f, 0.0f, 0.0f))));
			follow->SetSmoothSpeeds(100.0f, 100.0f);

			auto& cameraTransform = m_CameraEntity.GetComponent<Monsi::TransformComponent>();
			auto& playerTransform = m_CubeEntity.GetComponent<Monsi::TransformComponent>();
			cameraTransform.Translation = playerTransform.Translation + (playerTransform.Rotation * follow->GetPositionOffset());
			cameraTransform.Rotation = playerTransform.Rotation * follow->GetRotationOffset();
		}
	}

	m_Model = Monsi::CreateReference<Monsi::StaticModel>(MODEL_PATH "backpack/backpack.obj");

	m_ModelEntity = m_Scene->CreateEntity("Model");
	m_ModelEntity.AddComponent<Monsi::StaticModelComponent>(m_Model);
	m_ModelEntity.GetComponent<Monsi::TransformComponent>().Translation = { 0.0f, 3.0f, 0.0f };

	m_Unit.SetContext(m_Scene);
}

void GameLayer::OnLayerDetach()
{
}

void GameLayer::OnLayerUpdate(Monsi::TimeStep timestep)
{
	Monsi::FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_Scene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}

	m_FrameBuffer->Bind();
	Monsi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.12f, 1.0f });
	Monsi::RenderCommand::Clear();

	m_Scene->OnUpdate(timestep);

	m_FrameBuffer->Unbind();
	m_FrameBuffer->BlitToWindow();
}

void GameLayer::OnImGuiDraw()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize.x = 400.0f;

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

	float windowMinSizeWidth = style.WindowMinSize.x;
	float windowMinSizeHeight = style.WindowMinSize.y;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_None);

	style.WindowMinSize.x = windowMinSizeWidth;

	static bool first_time = true;
	if (first_time)
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		ImGuiID dock_id_main = dockspace_id;
		ImGuiID dock_id_left;
		ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_id_main);

		ImGuiID dock_id_left_top;
		ImGuiID dock_id_left_bottom;
		ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.5f, &dock_id_left_top, &dock_id_left_bottom);

		ImGui::DockBuilderDockWindow("Viewport", dock_id_main);
		ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left_top);
		ImGui::DockBuilderDockWindow("Animation Settings", dock_id_left_bottom);
		ImGui::DockBuilderDockWindow("Properties", dock_id_left_bottom);

		if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_id_main)) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}

		ImGui::DockBuilderFinish(dockspace_id);
	}

	m_Unit.OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f,0.0f });
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration);
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Monsi::Application::Get().GetImGuiLayer()->SetImGuiEventState(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void GameLayer::OnLayerEvent(Monsi::Event& event)
{
	Monsi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Monsi::KeyEventPressed>(ENGINE_BIND_EVENT_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnKeyPressed(Monsi::KeyEventPressed& event)
{

	if (event.getRepeat()) {
		return false;
	}

	return true;

}