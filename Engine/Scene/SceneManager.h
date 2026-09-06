#pragma  once

#include "Scene.h"

namespace Monsi {

	class SceneManager {

	public:
		SceneManager(const Reference<Scene>& Scene);

		void SaveScene(const std::string& filePath);
		bool LoadScene(const std::string& filePath);
	private:
		Reference<Scene> m_Scene;
	};

}