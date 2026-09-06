#include "MonsiPch.h"
#include "OpenGLShadowMap.h"
#include <glad/glad.h>

namespace Monsi {

	OpenGLShadowMap::OpenGLShadowMap(uint32_t width, uint32_t height)
	{
		m_ID = 0;
		m_ShadowMap = 0;

		m_Width = width;
		m_Height = height;

		if (m_ID) {
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_ShadowMap);
		}

		glGenFramebuffers(1, &m_ID);

		glGenTextures(1, &m_ShadowMap);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status: incomplete!");
	}

	OpenGLShadowMap::~OpenGLShadowMap()
	{
		if (m_ID != 0) {
			glDeleteFramebuffers(1, &m_ID);
		}

		if (m_ShadowMap != 0) {
			glDeleteTextures(1, &m_ShadowMap);
		}
	}

	void OpenGLShadowMap::Bind()
	{
		glGetIntegerv(GL_VIEWPORT, m_PrevViewport);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PrevFramebuffer);

		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 4.0f);
	}

	void OpenGLShadowMap::Unbind()
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glBindFramebuffer(GL_FRAMEBUFFER, m_PrevFramebuffer);
		glViewport(m_PrevViewport[0], m_PrevViewport[1], m_PrevViewport[2], m_PrevViewport[3]);
	}

	void OpenGLShadowMap::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		if (m_ID) {
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_ShadowMap);
		}

		glGenFramebuffers(1, &m_ID);

		glGenTextures(1, &m_ShadowMap);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status: incomplete!");
	}

	void OpenGLShadowMap::BindDepthTexture(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_ShadowMap);
	}

}