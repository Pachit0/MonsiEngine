#include "MonsiPch.h"
#include "VertexArray.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Monsi {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}

}