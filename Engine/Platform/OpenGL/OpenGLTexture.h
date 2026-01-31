#pragma once

#include "Texture.h"

namespace Monsi {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& ResourcePath);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t slot) const override;
	private:
		std::string m_ResourcePath;
		uint32_t m_Width, m_Height;
		uint32_t m_ID;
	};

}