#pragma once

#include "Core.h"

namespace Monsi {

	struct ShadowMapSettings {
		uint32_t Width = 4096, Height = 4096;
		float OrthoSize = 256.0f;
		float LightDistance = 50.0f;
		float NearPlane = -10.0f;
		float FarPlane = 1000.0f;
	};

	class ShadowMap {

	public:

		virtual ~ShadowMap() = default;

		virtual void Bind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetDepthTexture() const = 0;

		virtual void BindDepthTexture(uint32_t slot) const = 0;

		static Reference<ShadowMap> Create(uint32_t width, uint32_t height);
	};

}