#pragma once

#include "Texture.h"
#include <glad/glad.h>

namespace Monsi {

	class OpenGLCubeMapTexture : public CubeMapTexture {
	public:
		OpenGLCubeMapTexture(const std::array<std::string, 6>& ResourcePaths);
		virtual ~OpenGLCubeMapTexture();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_ID; };

		void modifyData(void* data, uint32_t size, uint32_t faceIndex) override;

		void Bind(uint32_t slot) const override;

		bool operator==(const Texture& other) const override
		{
			return m_ID == ((OpenGLCubeMapTexture&)other).m_ID;
		}

	private:
		uint32_t m_Width, m_Height;
		uint32_t m_ID;
		GLenum	m_DataFormat, m_OpenGLFormat;
	};

}