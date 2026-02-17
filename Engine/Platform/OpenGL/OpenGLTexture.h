#pragma once

#include "Texture.h"
#include <glad/glad.h>

namespace Monsi {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& ResourcePath);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void modifyData(void* data, uint32_t size) override;

		void Bind(uint32_t slot) const override;
	private:
		std::string m_ResourcePath;
		uint32_t m_Width, m_Height;
		uint32_t m_ID;
		GLenum	m_DataFormat, m_OpenGLFormat;
	};

}