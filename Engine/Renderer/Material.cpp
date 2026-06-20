#include "MonsiPch.h"
#include "Material.h"

namespace Monsi {

	Material::Material(const Reference<Texture>& diffuse, const glm::vec3& specular, float shininess)
		: m_DiffuseTexture(diffuse), m_SpecularColor(specular), m_Shininess(shininess)
	{
	}

	void Material::Bind(const Reference<Shader> shader)
	{
		shader->setVec3("u_Material.SpecularColor", m_SpecularColor);
		shader->setFloat("u_Material.Shininess", m_Shininess);

		if (m_DiffuseTexture)
		{
			m_DiffuseTexture->Bind(0);
			shader->setInt("u_Material.DiffuseMap", 0);
		}
	}

}