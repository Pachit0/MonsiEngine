#pragma once

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class Material {
	public:
		Material() = default;
		Material(const Material& other) = default;
		Material(Material&& other) noexcept = default;
		Material(const Reference<Material>& other);
		Material& operator=(Material& other) = default;
		Material& operator=(Material&& other) = default;

		void Bind(const Reference<Shader>& shader);

		glm::vec3 AmbientColor = glm::vec3(0.1f);
		glm::vec3 DiffuseColor = glm::vec3(1.0f);
		glm::vec3 SpecularColor = glm::vec3(1.0f);
		float Shininess = 32.0f;
		bool DoubleSided = false;

		Reference<Texture2D> DiffuseMap;
		Reference<Texture2D> SpecularMap;
		Reference<Texture2D> NormalMap;
	};

}