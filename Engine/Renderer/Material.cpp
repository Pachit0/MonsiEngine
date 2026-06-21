#include "MonsiPch.h"
#include "Material.h"

namespace Monsi {

	void Material::Bind(const Reference<Shader>& shader)
	{
		shader->setVec4("u_Material.DiffuseColor", DiffuseColor);
		shader->setVec3("u_Material.SpecularColor", SpecularColor);
		shader->setFloat("u_Material.Shininess", Shininess);

		if (DiffuseMap)
		{
			DiffuseMap->Bind(0);
			shader->setInt("u_Material.DiffuseMap", 0);
			shader->setFloat("u_Material.HasDiffuseMap", 1.0f);
		}
		else
		{
			shader->setFloat("u_Material.HasDiffuseMap", 0.0f);
		}

		if (SpecularMap)
		{
			SpecularMap->Bind(1);
			shader->setInt("u_Material.SpecularMap", 1);
			shader->setFloat("u_Material.HasSpecularMap", 1.0f);
		}
		else
		{
			shader->setFloat("u_Material.HasSpecularMap", 0.0f);
		}

		if (NormalMap)
		{
			NormalMap->Bind(2);
			shader->setInt("u_Material.NormalMap", 2);
			shader->setFloat("u_Material.HasNormalMap", 1.0f);
		}
		else
		{
			shader->setFloat("u_Material.HasNormalMap", 0.0f);
		}
	}

}