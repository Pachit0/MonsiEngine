#include "MonsiPch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Monsi {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) 
		: m_Specification(spec)
	{
		InvalidateFrameBuffer();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFrameBuffer::InvalidateFrameBuffer()
	{
		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Color);
		glBindTexture(GL_TEXTURE_2D, m_Color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Color, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Depth);
		glBindTexture(GL_TEXTURE_2D, m_Depth);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0,
		// 	GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_Depth, 0);

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status: incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}