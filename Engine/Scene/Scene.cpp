#include "MonsiPch.h"
#include <glm/glm.hpp>

#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace Monsi {

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.all_of<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e{ m_Registry.create(), this };
		e.AddComponent<TransformComponent>();

		auto& tag = e.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return e;
	}

	Entity Scene::CreateEntityEmpty()
	{
		return { m_Registry.create(), this };
	}


	void Scene::OnUpdate(TimeStep timeStep)
	{
		SceneCamera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (!mainCamera) return;

		if (mainCamera->GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			glm::vec3 cameraPos = glm::vec3((*cameraTransform)[3]);
			glm::mat4 viewProj = mainCamera->GetProjectionMatrix() * glm::inverse(*cameraTransform);

			SceneLighting sceneLighting;
			auto lightView = m_Registry.view<TransformComponent, LightComponent>();
			for (auto entity : lightView)
			{
				auto [transform, light] = lightView.get<TransformComponent, LightComponent>(entity);

				if (light.Type == LightComponent::LightType::Directional)
				{
					sceneLighting.MainLight.Direction = light.Direction;
					sceneLighting.MainLight.Color = light.Color;
					sceneLighting.MainLight.Intensity = light.Intensity;
				}
				else
				{
// 					if (sceneLighting.PointLights.size() >= LightingBuffer::MaxPointLights)
// 						continue;
					glm::vec3 position = glm::vec3(transform.Transform[3]);
					sceneLighting.PointLights.push_back({ position, light.Color, light.Intensity, light.Radius });
				}
			}
			Renderer3D::SetSceneLighting(sceneLighting);

			Renderer3D::Begin3D(viewProj, cameraPos);

			auto meshGroup = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto entity : meshGroup)
			{
				auto [transform, mesh] = meshGroup.get<TransformComponent, MeshComponent>(entity);
				Renderer3D::DrawMesh(mesh.MeshAsset.get(), transform.Transform, glm::vec4(1.0f));
			}

			auto modelGroup = m_Registry.view<TransformComponent, ModelComponent>();
			for (auto entity : modelGroup)
			{
				auto [transform, model] = modelGroup.get<TransformComponent, ModelComponent>(entity);
				Renderer3D::DrawModel(model.ModelAsset, transform.Transform, glm::vec4(1.0f));
			}

			Renderer3D::End3D();
		}
		else
		{
			Renderer2D::BeginScene2D(mainCamera->GetProjectionMatrix(), *cameraTransform);

			auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : spriteGroup)
			{
				auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene2D();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

}