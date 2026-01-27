#include "MonsiPch.h"
#include "Buffers.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Monsi {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}