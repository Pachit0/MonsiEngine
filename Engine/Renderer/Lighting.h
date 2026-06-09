#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Monsi {

	struct DirectionalLight
	{
		glm::vec3 Direction;
		glm::vec3 Color;
		float Intensity = 1.0f;
	};

	struct PointLight
	{
		glm::vec3 Position;
		glm::vec3 Color;
		float Intensity = 1.0f;
		float Radius = 10.0f;
	};

	struct SceneLighting
	{
		DirectionalLight MainLight;

		std::vector<PointLight> PointLights;
	};

	class LightingBuffer
	{
	public:
		static constexpr uint32_t MaxPointLights = 32;

		void SetLighting(const SceneLighting& lighting);
// 		void SetLighting(const DirectionalLight& lighting);
// 		void SetLighting(const PointLight& lighting);
		void Bind(const Reference<Shader>& shader) const;

	private:
		SceneLighting m_Lighting;
		std::vector<PointLight> m_VisiblePointLights;
		DirectionalLight m_DirLight;
	};

}