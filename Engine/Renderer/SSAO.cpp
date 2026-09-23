#include "MonsiPch.h"
#include "SSAO.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLSSAO.h"

namespace Monsi {

	Reference<SSAO> SSAO::Create(const SSAOSettings& settings)
	{
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateReference<OpenGLSSAO>(settings);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}