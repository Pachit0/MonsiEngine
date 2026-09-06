#include "MonsiPch.h"
#include "RenderInitializator.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Monsi {

	RenderTypeEnum RenderSystem::s_ActiveType = RenderTypeEnum::RendererNull;

	void RenderSystem::Init(RenderTypeEnum type)
	{
		s_ActiveType = type;

		switch (type)
		{
		case RenderTypeEnum::Renderer2D:
			Renderer2D::Init();
			break;
		case RenderTypeEnum::Renderer3D:
			Renderer3D::Init();
			break;
		case RenderTypeEnum::RendererNull:
		default:
			break;
		}
	}

	void RenderSystem::Shutdown()
	{
		switch (s_ActiveType)
		{
		case RenderTypeEnum::Renderer2D:

			Renderer2D::Shutdown(); 
			break;
		case RenderTypeEnum::Renderer3D:
			Renderer3D::Shutdown(); 
			break;
		default:
			break;
		}
		s_ActiveType = RenderTypeEnum::RendererNull;
	}

}