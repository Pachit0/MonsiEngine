#include "MonsiPch.h"
#include "OpenGLCubeMapTexture.h"
#include <stb_image.h>

namespace Monsi {

	OpenGLCubeMapTexture::OpenGLCubeMapTexture(const std::array<std::string, 6>& ResourcePaths)
	{
		ENGINE_PROFILER_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

		stbi_uc* data = nullptr;
		{
			std::string scopeName = "OpenGLTextureCubeMap -> stbi_load Face 0";
			ENGINE_PROFILER_SCOPE(scopeName.c_str());
			data = stbi_load(ResourcePaths[0].c_str(), &width, &height, &channels, 0);
		}

		ENGINE_ASSERT(data, "Failed to load cubemap face 0 image!");

		m_Width = width;
		m_Height = height;

		GLenum OpenGLFormat = 0, dataFormat = 0;
		if (channels == 4) {
			OpenGLFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			OpenGLFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_DataFormat = dataFormat;
		m_OpenGLFormat = OpenGLFormat;
		ENGINE_ASSERT(OpenGLFormat != 0 && dataFormat != 0, "Format isn't supported!");

		glTextureStorage2D(m_ID, 1, OpenGLFormat, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage3D(m_ID, 0, 0, 0, 0, m_Width, m_Height, 1, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		int i = 1;
		do {
			{
				std::string scopeName = "OpenGLTextureCubeMap -> stbi_load Face " + std::to_string(i);
				ENGINE_PROFILER_SCOPE(scopeName.c_str());
				data = stbi_load(ResourcePaths[i].c_str(), &width, &height, &channels, 0);
			}
			ENGINE_ASSERT(data, "Failed to load cubemap face image!");
			ENGINE_ASSERT(width == m_Width && height == m_Height, "Cubemap faces must have identical dimensions!");

			glTextureSubImage3D(m_ID, 0, 0, 0, i, m_Width, m_Height, 1, dataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			i++;
		} while (i < 6);
	}

	OpenGLCubeMapTexture::~OpenGLCubeMapTexture()
	{
		ENGINE_PROFILER_FUNCTION();
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLCubeMapTexture::modifyData(void* data, uint32_t size, uint32_t faceIndex)
	{
		ENGINE_PROFILER_FUNCTION();
		ENGINE_ASSERT(faceIndex < 6, "Invalid face index!");

		uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		ENGINE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Must be the entire texture face for the data!");
		glTextureSubImage3D(m_ID, 0, 0, 0, faceIndex, m_Width, m_Height, 1, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLCubeMapTexture::Bind(uint32_t slot) const
	{
		ENGINE_PROFILER_FUNCTION();
		glBindTextureUnit(slot, m_ID);
	}

}