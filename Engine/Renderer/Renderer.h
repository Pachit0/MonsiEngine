#pragma once

#include "Renderer/RenderCommand.h"

namespace Monsi {

	class Renderer {
	public:
		static void Begin();
		static void End();
		static void Sumbit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	};

}