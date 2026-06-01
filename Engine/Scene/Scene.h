#pragma once

#include "entt.hpp"
#include "TimeStep.h"

namespace Monsi {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityEmpty();

		void OnUpdate(TimeStep timeStep);
		void OnViewportResize(uint32_t width, uint32_t height);
		
	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		entt::registry m_Registry;

		friend class Entity;
	};

}