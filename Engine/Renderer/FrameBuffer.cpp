#include "MonsiPch.h"
#include "FrameBuffer.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Monsi {

	Reference<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec& spec)
	{
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateReference<OpenGLFrameBuffer>(spec);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}