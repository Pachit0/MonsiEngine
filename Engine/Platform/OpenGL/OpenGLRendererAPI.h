#pragma once

#include "Renderer/RendererAPI.h"

namespace Monsi {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		void Init() override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Reference<VertexArray>& vertexArray) override;
	private:

	};

}