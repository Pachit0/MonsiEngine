#pragma once

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class Material {
	public:

		Material(const Reference<Texture>& diffuse, const glm::vec3& specular, float shininess);
		void Bind(const Reference<Shader> shader);

		void SetDiffuseTexture(const Reference<Texture>& texture) { m_DiffuseTexture = texture; }
		void SetSpecularColor(const glm::vec3& color) { m_SpecularColor = color; }
		void SetShininess(float shininess) { m_Shininess = shininess; }

	private:
		Reference<Texture> m_DiffuseTexture;
		glm::vec3 m_SpecularColor;
		float m_Shininess;
	};

}