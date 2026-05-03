#pragma once

#include "entt.hpp"
#include "TimeStep.h"

namespace Monsi {

	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(TimeStep timeStep);
		
		entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;
	};

}