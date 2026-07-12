#pragma once

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Scene/Scene.h"
#include "Entity.h"
#include <glm/glm.hpp>

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

		//temporary solution
		glm::vec3 m_CachedEulerDegrees = glm::vec3(0.0f);
		bool m_EditingRotation = false;
	};


}