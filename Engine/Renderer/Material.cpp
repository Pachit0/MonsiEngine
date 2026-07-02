#include "MonsiPch.h"
#include "Material.h"

namespace Monsi {

	void Material::Bind(const Reference<Shader>& shader)
	{
		shader->setVec3("material.ambient", AmbientColor);
		shader->setVec3("material.diffuse", DiffuseColor);
		shader->setVec3("material.specular", SpecularColor);
		shader->setFloat("material.shininess", Shininess);

		if (DiffuseMap)
		{
			DiffuseMap->Bind(0);
			shader->setInt("texture_diffuse1", 0);
			shader->setFloat("material.hasDiffuseMap", 1.0f);
		}
		else
		{
			shader->setFloat("material.hasDiffuseMap", 0.0f);
		}

		if (SpecularMap)
		{
			SpecularMap->Bind(1);
			shader->setInt("material.specularMap", 1);
			shader->setFloat("material.hasSpecularMap", 1.0f);
		}
		else
		{
			shader->setFloat("material.hasSpecularMap", 0.0f);
		}

		if (NormalMap)
		{
			NormalMap->Bind(2);
			shader->setInt("material.normalMap", 2);
			shader->setFloat("material.hasNormalMap", 1.0f);
		}
		else
		{
			shader->setFloat("material.hasNormalMap", 0.0f);
		}
	}

}