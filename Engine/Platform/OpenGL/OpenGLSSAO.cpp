#include "MonsiPch.h"
#include "OpenGLSSAO.h"

#include <random>

namespace Monsi {

	static float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	OpenGLSSAO::OpenGLSSAO(const SSAOSettings& settings)
		: m_Settings(settings)
	{
		GenerateKernelAndNoise();
		Init();
	}

	OpenGLSSAO::~OpenGLSSAO()
	{
		Cleanup();
	}

	void OpenGLSSAO::SetSettings(const SSAOSettings& settings)
	{
		bool kernelSizeChanged = (m_Settings.KernelSize != settings.KernelSize);
		bool resolutionChanged = (m_Settings.Width != settings.Width || m_Settings.Height != settings.Height);

		m_Settings = settings;

		if (kernelSizeChanged) {
			GenerateKernelAndNoise();
		}

		if (resolutionChanged) {
			Init();
		}
	}

	void OpenGLSSAO::GenerateKernelAndNoise()
	{
		std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
		std::default_random_engine generator;

		m_SSAOKernel.clear();
		for (int i = 0; i < m_Settings.KernelSize; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);

			float scale = static_cast<float>(i) / static_cast<float>(m_Settings.KernelSize);
			scale = Lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;

			m_SSAOKernel.push_back(sample);
		}

		std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f);
			ssaoNoise.push_back(noise);
		}

		if (m_NoiseTexture) {
			glDeleteTextures(1, &m_NoiseTexture);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_NoiseTexture);
		glTextureStorage2D(m_NoiseTexture, 1, GL_RGBA32F, 4, 4);
		glTextureSubImage2D(m_NoiseTexture, 0, 0, 0, 4, 4, GL_RGB, GL_FLOAT, ssaoNoise.data());

		glTextureParameteri(m_NoiseTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_NoiseTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_NoiseTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_NoiseTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLSSAO::Init()
	{
		Cleanup();

		glCreateFramebuffers(1, &m_SSAOFBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_SSAOColorBuffer);
		glTextureStorage2D(m_SSAOColorBuffer, 1, GL_R8, m_Settings.Width, m_Settings.Height);

		glTextureParameteri(m_SSAOColorBuffer, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_SSAOColorBuffer, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_SSAOColorBuffer, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_SSAOColorBuffer, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glNamedFramebufferTexture(m_SSAOFBO, GL_COLOR_ATTACHMENT0, m_SSAOColorBuffer, 0);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(m_SSAOFBO, 1, drawBuffers);

		ENGINE_ASSERT(glCheckNamedFramebufferStatus(m_SSAOFBO, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Framebuffer status: incomplete!");

		glCreateFramebuffers(1, &m_SSAOBlurFBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_SSAOBlurColorBuffer);
		glTextureStorage2D(m_SSAOBlurColorBuffer, 1, GL_R8, m_Settings.Width, m_Settings.Height);

		glTextureParameteri(m_SSAOBlurColorBuffer, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_SSAOBlurColorBuffer, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_SSAOBlurColorBuffer, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_SSAOBlurColorBuffer, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glNamedFramebufferTexture(m_SSAOBlurFBO, GL_COLOR_ATTACHMENT0, m_SSAOBlurColorBuffer, 0);
		glNamedFramebufferDrawBuffers(m_SSAOBlurFBO, 1, drawBuffers);

		ENGINE_ASSERT(glCheckNamedFramebufferStatus(m_SSAOBlurFBO, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Blur Framebuffer status: incomplete!");
	}

	void OpenGLSSAO::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;
		if (m_Settings.Width == width && m_Settings.Height == height) return;

		m_Settings.Width = width;
		m_Settings.Height = height;

		Init();
	}

	void OpenGLSSAO::BindSSAOFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);
		glViewport(0, 0, m_Settings.Width, m_Settings.Height);
	}

	void OpenGLSSAO::BindBlurFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOBlurFBO);
		glViewport(0, 0, m_Settings.Width, m_Settings.Height);
	}

	void OpenGLSSAO::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLSSAO::Cleanup()
	{
		if (m_SSAOFBO) {
			glDeleteFramebuffers(1, &m_SSAOFBO);
			m_SSAOFBO = 0;
		}
		if (m_SSAOColorBuffer) {
			glDeleteTextures(1, &m_SSAOColorBuffer);
			m_SSAOColorBuffer = 0;
		}
		if (m_SSAOBlurFBO) {
			glDeleteFramebuffers(1, &m_SSAOBlurFBO);
			m_SSAOBlurFBO = 0;
		}
		if (m_SSAOBlurColorBuffer) {
			glDeleteTextures(1, &m_SSAOBlurColorBuffer);
			m_SSAOBlurColorBuffer = 0;
		}
	}

}