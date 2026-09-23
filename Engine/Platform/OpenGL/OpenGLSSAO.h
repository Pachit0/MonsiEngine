#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "SSAO.h"

namespace Monsi {

	class OpenGLSSAO : public SSAO
	{
	public:
		OpenGLSSAO(const SSAOSettings& settings);
		~OpenGLSSAO() override;

		void BindSSAOFramebuffer() override;
		void BindBlurFramebuffer() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetSSAOTextureID() const override { return m_SSAOColorBuffer; }
		uint32_t GetBlurredTextureID() const override { return m_SSAOBlurColorBuffer; }

		const std::vector<glm::vec3>& GetKernel() const override { return m_SSAOKernel; }
		uint32_t GetNoiseTextureID() const override { return m_NoiseTexture; }

		const SSAOSettings& GetSettings() const override { return m_Settings; }
		void SetSettings(const SSAOSettings& settings) override;

	private:
		void Init();
		void Cleanup();
		void GenerateKernelAndNoise();

	private:
		SSAOSettings m_Settings;

		uint32_t m_SSAOFBO = 0;
		uint32_t m_SSAOColorBuffer = 0;

		uint32_t m_SSAOBlurFBO = 0;
		uint32_t m_SSAOBlurColorBuffer = 0;

		uint32_t m_NoiseTexture = 0;
		std::vector<glm::vec3> m_SSAOKernel;
	};

}