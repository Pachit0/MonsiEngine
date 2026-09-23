#include "MonsiPch.h"
#include "OpenGLShadowMap.h"
#include <glad/glad.h>

namespace Monsi {

	OpenGLShadowMap::OpenGLShadowMap(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		Init();
	}

	OpenGLShadowMap::~OpenGLShadowMap()
	{
		Cleanup();
	}

	void OpenGLShadowMap::Init()
	{
		Cleanup();

		glCreateFramebuffers(1, &m_ID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ShadowMap);

		glTextureStorage2D(m_ShadowMap, 1, GL_DEPTH_COMPONENT24, m_Width, m_Height);

		glTextureParameteri(m_ShadowMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_ShadowMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_ShadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(m_ShadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTextureParameterfv(m_ShadowMap, GL_TEXTURE_BORDER_COLOR, borderColor);

		glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTextureParameteri(m_ShadowMap, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, m_ShadowMap, 0);

		glNamedFramebufferDrawBuffer(m_ID, GL_NONE);
		glNamedFramebufferReadBuffer(m_ID, GL_NONE);

		ENGINE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Shadow map framebuffer status: incomplete!");
	}

	void OpenGLShadowMap::Cleanup()
	{
		if (m_ID != 0) {
			glDeleteFramebuffers(1, &m_ID);
			m_ID = 0;
		}

		if (m_ShadowMap != 0) {
			glDeleteTextures(1, &m_ShadowMap);
			m_ShadowMap = 0;
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
		if (m_Width == width && m_Height == height) {
			return;
		}

		m_Width = width;
		m_Height = height;

		Init();
	}

	void OpenGLShadowMap::BindDepthTexture(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_ShadowMap);
	}

}