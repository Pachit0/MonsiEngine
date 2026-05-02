#pragma once

#include "entt.hpp"

namespace Monsi {

	class Scene {
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;
	};

}