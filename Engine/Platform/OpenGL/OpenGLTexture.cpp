#include "MonsiPch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>



namespace Monsi {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& ResourcePath) : m_ResourcePath(ResourcePath) {
		
		ENGINE_PROFILER_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		{
			ENGINE_PROFILER_SCOPE("OpenGLTexture2D::OpenGLTexture2D(Resource) -> stbi_load");
			data = stbi_load(ResourcePath.c_str(), &width, &height, &channels, 0);
		}
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

		m_DataFormat = dataFormat;
		m_OpenGLFormat = OpenGLFormat;

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

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
	{
		ENGINE_PROFILER_FUNCTION();
		m_DataFormat = GL_RGBA;
		m_OpenGLFormat = GL_RGBA8;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_OpenGLFormat, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ENGINE_PROFILER_FUNCTION();
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::modifyData(void* data, uint32_t size)
	{
		ENGINE_PROFILER_FUNCTION();
		uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		ENGINE_ASSERT("Must be the entire texture for the data!", size == m_Width * m_Height * bytesPerPixel);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ENGINE_PROFILER_FUNCTION();
		glBindTextureUnit(slot, m_ID);
	}

}