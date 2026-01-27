#include "MonsiPch.h"
#include "Renderer/RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Monsi {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}