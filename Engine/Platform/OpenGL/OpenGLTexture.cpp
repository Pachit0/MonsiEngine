#include "MonsiPch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>
#include <glad/glad.h>


namespace Monsi {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& ResourcePath) : m_ResourcePath(ResourcePath) {
		
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(ResourcePath.c_str(), &width, &height, &channels, 0);
		ENGINE_ASSERT(data, "Failed to load image!");
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

		ENGINE_ASSERT(OpenGLFormat & dataFormat, "Format isn't supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, OpenGLFormat, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data); //upload to GPU

		stbi_image_free(data); //unload from CPU
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_ID);
	}

}