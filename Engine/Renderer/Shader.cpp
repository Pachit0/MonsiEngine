#include "MonsiPch.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "glad/glad.h"

namespace Monsi {

	Shader* Shader::Create(const std::string& vertexPath, const std::string& fragmentPath) {
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::None: ENGINE_ASSERT(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::OpenGL: return new OpenGLShader(vertexPath, fragmentPath);
		}
	ENGINE_ASSERT(false, "Unknown Shader!");
	return nullptr;
	}

}