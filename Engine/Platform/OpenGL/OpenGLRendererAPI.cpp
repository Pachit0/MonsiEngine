#include "MonsiPch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Monsi {

	void OpenGLRendererAPI::Init() {

		ENGINE_PROFILER_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t widht, uint32_t height) {
		glViewport(x, y, widht, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetDepthFunc(DepthComp comp)
	{
		ENGINE_PROFILER_FUNCTION();

		switch (comp)
		{
		case DepthComp::LESS:   glDepthFunc(GL_LESS); break;
		case DepthComp::LEQUAL: glDepthFunc(GL_LEQUAL); break;
		}
	}

	void OpenGLRendererAPI::DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount) {
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const Reference<VertexArray>& vertexArray, uint32_t indexCount, uint32_t instanceCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}