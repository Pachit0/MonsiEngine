#pragma once

#include "ShadowMap.h"

namespace Monsi {

	class OpenGLShadowMap : public ShadowMap {
	public:
		OpenGLShadowMap();
		~OpenGLShadowMap() override;
		bool Init(uint32_t width, uint32_t height) override;
		void Bind() override;
		void Unbind() override;

		uint32_t GetDepthTexture() const override { return m_ShadowMap; }
		void BindDepthTexture(uint32_t slot) const override;

	private:
		uint32_t m_ID;
		uint32_t m_ShadowMap;
		uint32_t m_Width, m_Height;
		int32_t m_PrevViewport[4] = { 0, 0, 0, 0 };
	};

}