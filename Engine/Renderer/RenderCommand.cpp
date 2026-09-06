#include "MonsiPch.h"
#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Monsi {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}