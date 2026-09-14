#include "MonsiPch.h"
#include "StorageBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLStorageBuffer.h"

namespace Monsi {

	Reference<StorageBuffer> StorageBuffer::Create(uint32_t size) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateReference<OpenGLStorageBuffer>(size);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}