#include "MonsiPch.h"
#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Monsi {

	Reference<Texture2D> Texture2D::Create(const std::string& ResourcePath) {
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::API::None: ENGINE_ASSERT(false, "RendererAPI::None!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(ResourcePath);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}