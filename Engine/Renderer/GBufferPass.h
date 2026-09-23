#pragma once

#include "FrameBuffer.h"
#include "Shader.h"
#include "StaticModel.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Monsi {

	class GBufferPass {
	public:
		void Init();
		void Shutdown();

		void Begin(const glm::mat4& view, const glm::mat4& projection, const Reference<FrameBuffer>& gBuffer);
		void SubmitMesh(const StaticMesh* meshPtr, const glm::mat4& transform);
		void SubmitModel(const Reference<StaticModel>& model, const glm::mat4& transform);
		void End();

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t Triangles = 0;
		};
		const Stats& GetStats() const { return m_Stats; }
		void ResetStats() { m_Stats = {}; }

	private:
		Reference<Shader> m_GBufferShader;
		Reference<FrameBuffer> m_ActiveGBuffer;
		glm::mat4 m_View{ 1.0f };
		glm::mat4 m_Projection{ 1.0f };
		Stats m_Stats;

		GLint m_PreviousFramebuffer = 0;
		GLint m_PreviousViewport[4] = { 0, 0, 0, 0 };
	};

}