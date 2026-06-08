#pragma once

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Scene/Scene.h"
#include "Entity.h"


namespace Monsi {

	class SceneHierarchyUnit {
	public:
		SceneHierarchyUnit() = default;
		SceneHierarchyUnit(const Reference<Scene>& scene);

		void SetContext(const Reference<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);

	private:
		friend class Scene;
		Reference<Scene> m_Scene;

		Entity m_Selected;
	};


}