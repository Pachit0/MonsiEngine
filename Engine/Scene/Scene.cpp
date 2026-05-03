#include "MonsiPch.h"
#include "Scene.h"
#include "glm/glm.hpp"

namespace Monsi {

	static void OnTransformConstruct(entt::registry& registry, entt::entity& entity) {

	}

	Scene::Scene()
	{
		struct MeshComponent {
			// TODO for 3D renderer
		};


		struct TransformComponent {
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent& other) = default;
			TransformComponent(const glm::mat4& transform) : Transform(transform) {}

			operator const glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group) {
			auto& [transform, mesh] = m_Registry.get<MeshComponent>(entity);


		}
	}

	Scene::~Scene()
	{

	}


}