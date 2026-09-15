#pragma once

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Scene/Scene.h"
#include "Entity.h"
#include <glm/glm.hpp>

namespace Monsi {

	class SceneHierarchyUnit {
	public:
		SceneHierarchyUnit() = default;
		SceneHierarchyUnit(const Reference<Scene>& scene);

		void SetContext(const Reference<Scene>& scene);
		void SetFPS(float fps) { m_FPS = fps; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		void DrawAnimatedModelWindow();

		void DrawOptionsMenu();
		void DrawInfoWindow();
		void DrawSettingsWindow();

	private:
		Reference<Scene> m_Scene;
		Entity m_Selected;
	
	private:
		glm::vec3 preUIEuler = glm::vec3(0.0f);
		bool m_ShowInfoWindow = false;
		bool m_ShowSettingsWindow = false;
		float m_FPS = 0.0f;
	};


}