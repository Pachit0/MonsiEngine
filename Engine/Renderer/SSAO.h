#pragma once

#include "Core.h"
#include <glm/glm.hpp>

namespace Monsi {

	struct SSAOSettings
	{
		uint32_t Width = 1600;
		uint32_t Height = 900;
		int KernelSize = 64;
		float Radius = 0.5f;
		float Bias = 0.025f;
	};

	class SSAO
	{
	public:
		virtual ~SSAO() = default;

		virtual void BindSSAOFramebuffer() = 0;
		virtual void BindBlurFramebuffer() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetSSAOTextureID() const = 0;
		virtual uint32_t GetBlurredTextureID() const = 0;

		virtual const std::vector<glm::vec3>& GetKernel() const = 0;
		virtual uint32_t GetNoiseTextureID() const = 0;

		virtual const SSAOSettings& GetSettings() const = 0;
		virtual void SetSettings(const SSAOSettings& settings) = 0;

		static Reference<SSAO> Create(const SSAOSettings& settings = SSAOSettings());
	};

}