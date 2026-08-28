#pragma once

#include "entt.hpp"
#include "TimeStep.h"
#include "ShadowMap.h"

namespace Monsi {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string(), bool hasTransform = true);
		Entity CreateEntityEmpty();

		void RemoveEntity(Entity entity);
		void OnUpdate(TimeStep timeStep);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnAddComponent(Entity entity, T& component);

	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		entt::registry m_Registry;

		Reference<ShadowMap> m_ShadowMap;

		friend class Entity;
		friend class SceneManager;
		friend class SceneHierarchyUnit;
	};

}