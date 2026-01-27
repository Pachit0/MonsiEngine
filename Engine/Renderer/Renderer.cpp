#include "MonsiPch.h"
#include "Renderer.h"

namespace Monsi {

	void Renderer::Begin() {

	}

	void Renderer::End() {

	}

	void Renderer::Sumbit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}