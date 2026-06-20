#include "MonsiPch.h"
#include "Lighting.h"

namespace Monsi {

	void LightingBuffer::SetLighting(const SceneLighting& lighting)
	{
		m_Lighting = lighting;
	}

	void LightingBuffer::AddPointLighting(const glm::vec3& position, const glm::vec3& color, float intensity, float radius)
	{
		m_Lighting.PointLights.emplace_back(PointLight{position, color, intensity, radius});
	}

// 	void LightingBuffer::SetLighting(const DirectionalLight& lighting)
// 	{
// 
// 	}

	void LightingBuffer::Bind(const Reference<Shader>& shader) const
	{

		if (!shader) return;

		shader->setVec3("u_MainLight.Direction", m_Lighting.MainLight.Direction);
		shader->setVec3("u_MainLight.Color", m_Lighting.MainLight.Color);
		shader->setFloat("u_MainLight.Intensity", m_Lighting.MainLight.Intensity);

		uint32_t count = std::min((uint32_t)m_Lighting.PointLights.size(), MaxPointLights);
		shader->setInt("u_PointLightCount", (int)count);

		//probably the worst way to go around the problem :)
		for (uint32_t i = 0; i < count; i++)
		{
			const auto& light = m_Lighting.PointLights[i];

			std::string base = "u_PointLights[" + std::to_string(i) + "]";

			shader->setVec3(base + ".Position", light.Position);
			shader->setVec3(base + ".Color", light.Color);
			shader->setFloat(base + ".Intensity", light.Intensity);
			shader->setFloat(base + ".Radius", light.Radius);
		}
	}

}