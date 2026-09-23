#include "MonsiPch.h"
#include "OpenGLFrameBuffer.h"

namespace Monsi {

	static GLenum ToGLInternalFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::RGBA8:   return GL_RGBA8;
		case FrameBufferTextureFormat::RGBA16F: return GL_RGBA16F;
		}

		ENGINE_ASSERT(false, "Unknown color attachment format!");
		return GL_RGBA8;
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec)
		: m_Specification(spec)
	{
		m_ColorAttachmentFormats = spec.Attachments;
		if (m_ColorAttachmentFormats.empty())
			m_ColorAttachmentFormats.push_back(FrameBufferTextureFormat::RGBA8);

		InvalidateFrameBuffer();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Cleanup();
	}

	void OpenGLFrameBuffer::InvalidateFrameBuffer()
	{
		Cleanup();

		glCreateFramebuffers(1, &m_ID);

		m_ColorAttachments.clear();
		std::vector<GLenum> drawBuffers;
		drawBuffers.reserve(m_ColorAttachmentFormats.size());

		for (size_t i = 0; i < m_ColorAttachmentFormats.size(); ++i)
		{
			GLuint colorID = GenerateColorAttachmentDSA(m_ColorAttachmentFormats[i], m_Specification.Width, m_Specification.Height);
			glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + (GLenum)i, colorID, 0);

			m_ColorAttachments.push_back(colorID);
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + (GLenum)i);
		}

		glNamedFramebufferDrawBuffers(m_ID, (GLsizei)drawBuffers.size(), drawBuffers.data());

		m_Depth = GenerateDepthBufferDSA(m_Specification.Width, m_Specification.Height);
		glNamedFramebufferTexture(m_ID, GL_DEPTH_STENCIL_ATTACHMENT, m_Depth, 0);

		ENGINE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status: incomplete!");
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;
		if (m_Specification.Width == width && m_Specification.Height == height) return;

		m_Specification.Width = width;
		m_Specification.Height = height;
		InvalidateFrameBuffer();
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::BlitToWindow()
	{
		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0);
		glBlitNamedFramebuffer(m_ID, 0, 0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	GLuint OpenGLFrameBuffer::GenerateColorAttachmentDSA(FrameBufferTextureFormat format, uint32_t width, uint32_t height)
	{
		GLenum internalFormat = ToGLInternalFormat(format);
		// G-Buffer position/normal targets store raw data - must not be filtered.
		GLint filter = (format == FrameBufferTextureFormat::RGBA16F) ? GL_NEAREST : GL_LINEAR;

		GLuint textureID = 0;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, internalFormat, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return textureID;
	}

	GLuint OpenGLFrameBuffer::GenerateDepthBufferDSA(uint32_t width, uint32_t height)
	{
		GLuint textureID = 0;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, GL_DEPTH24_STENCIL8, width, height);

		return textureID;
	}

	void OpenGLFrameBuffer::Cleanup()
	{
		if (m_ID != 0) {
			glDeleteFramebuffers(1, &m_ID);
			m_ID = 0;
		}

		if (!m_ColorAttachments.empty()) {
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			m_ColorAttachments.clear();
		}

		if (m_Depth != 0) {
			glDeleteTextures(1, &m_Depth);
			m_Depth = 0;
		}
	}

}