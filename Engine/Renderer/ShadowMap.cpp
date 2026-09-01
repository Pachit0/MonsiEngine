#include "MonsiPch.h"
#include "ShadowMap.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShadowMap.h"

namespace Monsi {

	Reference<ShadowMap> ShadowMap::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateReference<OpenGLShadowMap>(width, height);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}