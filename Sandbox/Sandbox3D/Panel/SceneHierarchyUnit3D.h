#pragma once

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Scene/Scene.h"
#include "Entity.h"


namespace Monsi {

	class SceneHierarchyUnit3D {
	public:
		SceneHierarchyUnit3D() = default;
		SceneHierarchyUnit3D(const Reference<Scene>& scene);

		void SetContext(const Reference<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Reference<Scene> m_Scene;

		Entity m_Selected;
	};


}