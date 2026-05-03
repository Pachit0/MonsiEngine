#include "MonsiPch.h"
#include <glm/glm.hpp>

#include "Renderer2D.h"
#include "Scene.h"
#include "Components.h"

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


	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(TimeStep timeStep)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::drawQuad(transform, sprite.Color);
		}
	}

}