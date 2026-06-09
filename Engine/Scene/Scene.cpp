#include "MonsiPch.h"
#include <glm/glm.hpp>

#include "Renderer2D.h"
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

		// 		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		// 		for (auto entity : group) {
		// 			auto& [transform, mesh] = m_Registry.get<MeshComponent>(entity);
		// 
		// 
		// 		}
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
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}


		if (mainCamera)
		{
			Renderer2D::BeginScene2D(mainCamera->GetProjectionMatrix(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

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