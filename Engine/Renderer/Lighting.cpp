#include "MonsiPch.h"
#include "Lighting.h"

namespace Monsi {

	LightingBuffer::LightingBuffer()
	{
		m_UniformNameCache.resize(MaxPointLights);
		for (uint32_t i = 0; i < MaxPointLights; i++)
		{
			std::string base = "u_PointLights[" + std::to_string(i) + "]";
			m_UniformNameCache[i].Position = base + ".Position";
			m_UniformNameCache[i].Color = base + ".Color";
			m_UniformNameCache[i].Intensity = base + ".Intensity";
			m_UniformNameCache[i].Radius = base + ".Radius";
		}
	}

	void LightingBuffer::Clear()
	{
		m_Lighting.PointLights.clear();
	}

	void LightingBuffer::SetLighting(const SceneLighting& lighting)
	{
		m_Lighting = lighting;
	}

	void LightingBuffer::AddPointLighting(const glm::vec3& position, const glm::vec3& color, float intensity, float radius)
	{
		m_Lighting.PointLights.emplace_back(PointLight{ position, color, intensity, radius });
	}

	void LightingBuffer::Bind(const Reference<Shader>& shader) const
	{
		if (!shader) return;

		shader->setVec3("u_MainLight.Direction", m_Lighting.MainLight.Direction);
		shader->setVec3("u_MainLight.Color", m_Lighting.MainLight.Color);
		shader->setFloat("u_MainLight.Intensity", m_Lighting.MainLight.Intensity);

		uint32_t count = std::min((uint32_t)m_Lighting.PointLights.size(), MaxPointLights);
		shader->setInt("u_PointLightCount", (int)count);

		for (uint32_t i = 0; i < count; i++)
		{
			const auto& light = m_Lighting.PointLights[i];
			const auto& names = m_UniformNameCache[i];

			shader->setVec3(names.Position, light.Position);
			shader->setVec3(names.Color, light.Color);
			shader->setFloat(names.Intensity, light.Intensity);
			shader->setFloat(names.Radius, light.Radius);
		}
	}

	SceneLighting::SceneLighting()
	{
		PointLights.reserve(LightingBuffer::MaxPointLights);
	}

}