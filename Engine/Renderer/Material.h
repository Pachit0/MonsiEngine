#pragma once

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class Material {
	public:
		Material() = default;

		void Bind(const Reference<Shader>& shader);

		glm::vec4 DiffuseColor = glm::vec4(1.0f);
		glm::vec3 SpecularColor = glm::vec3(1.0f);
		float Shininess = 32.0f;

		Reference<Texture2D> DiffuseMap;
		Reference<Texture2D> SpecularMap;
		Reference<Texture2D> NormalMap;
	};

}