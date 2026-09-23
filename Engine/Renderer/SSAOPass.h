#pragma once

#include "SSAO.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class SSAOPass {
	public:
		void Init();
		void Shutdown();

		void DrawSSAO(const Reference<SSAO>& ssaoInstance, const Reference<FrameBuffer>& gBuffer, const glm::mat4& projection);
		void Resize(uint32_t width, uint32_t height, const Reference<SSAO>& ssaoInstance);

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t Triangles = 0;
		};
		const Stats& GetStats() const { return m_Stats; }
		void ResetStats() { m_Stats = {}; }

	private:
		void RenderScreenQuad();

	private:
		static constexpr uint32_t PositionTextureSlot = 0;
		static constexpr uint32_t NormalTextureSlot = 1;
		static constexpr uint32_t NoiseTextureSlot = 2;
		static constexpr uint32_t SSAOTextureSlot = 0;

		Reference<Shader> m_SSAOShader;
		Reference<Shader> m_SSAOBlurShader;

		Reference<VertexArray> m_QuadVAO;
		Reference<VertexBuffer> m_QuadVBO;

		Stats m_Stats;
	};

}