#include "MonsiPch.h"
#include "VertexArray.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Monsi {

	Reference<VertexArray> VertexArray::Create() {

		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}

}