#pragma once

#include "Core.h"

namespace Monsi {

	class ShadowMap {

	public:

		virtual ~ShadowMap() = default;
		virtual bool Init(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetDepthTexture() const = 0;

		virtual void BindDepthTexture(uint32_t slot) const = 0;

		static Reference<ShadowMap> Create(uint32_t width, uint32_t height);
	};

}