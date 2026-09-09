#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Monsi {

	class SkyBoxPass {

	public:
		SkyBoxPass() = default;

		void Init();
		void Shutdown();
		void DrawSkybox(const glm::mat4& view, const glm::mat4& projection, const Reference<CubeMapTexture>& skyboxTexture);

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t Triangles = 0;
		};
		const Stats& GetStats() const { return m_Stats; }
		void ResetStats() { m_Stats = {}; }
	private:

		Reference<VertexArray> m_SkyboxVA;
		Reference<VertexBuffer> m_SkyboxVertexBuffer;
		Reference<IndexBuffer> m_SkyboxIB;
		Reference<Shader> m_SkyboxShader;

		Stats m_Stats;
	};

}