#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Monsi {

	struct DirectionalLight
	{
		//default values for the default lighting source of the 3D scene
		glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		glm::vec3 Color = glm::vec3(1.0f, 0.95f, 0.9f);
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


	struct PointLightUniformStrings {
		std::string Position;
		std::string Color;
		std::string Intensity;
		std::string Radius;
	};

	class LightingBuffer
	{
	public:
		LightingBuffer();
		void Clear();
		void SetLighting(const SceneLighting& lighting);
		void AddPointLighting(const glm::vec3& position, const glm::vec3& color, float intensity, float radius);
		void Bind(const Reference<Shader>& shader) const;

		static constexpr uint32_t MaxPointLights = 32;
	private:
		SceneLighting m_Lighting;
		std::vector<PointLightUniformStrings> m_UniformNameCache;
	};

}