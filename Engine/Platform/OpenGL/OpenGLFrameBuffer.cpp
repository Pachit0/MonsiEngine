#include "MonsiPch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Monsi {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) 
		: m_Specification(spec), m_ID(0), m_Color(0), m_Depth(0)
	{
		InvalidateFrameBuffer();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_Depth);
		glDeleteTextures(1, &m_Color);
	}

	void OpenGLFrameBuffer::InvalidateFrameBuffer()
	{
		if (m_ID) {
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_Depth);
			glDeleteTextures(1, &m_Color);
		}

		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Color);
		glBindTexture(GL_TEXTURE_2D, m_Color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Color, 0);
		GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Depth);
		glBindTexture(GL_TEXTURE_2D, m_Depth);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_Depth, 0);

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status: incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
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

}