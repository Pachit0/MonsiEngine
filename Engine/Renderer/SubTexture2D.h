#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Monsi {

	class SubTexture2D {

	public:
		SubTexture2D(const Reference<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Reference<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTextureCoords() const { return m_TexCoords; }


		static Reference<SubTexture2D> CreateSubTexture(const Reference<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Reference<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};


}