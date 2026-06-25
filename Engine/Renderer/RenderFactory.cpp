#include "MonsiPch.h"
#include "RenderFactory.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Monsi {

	void RenderFactory::InitRenderType(RenderTypeEnum type)
	{
		switch (type)
		{
		case RenderTypeEnum::Renderer2D :
		{
			Renderer2D::Init();
			break;
		}

		case RenderTypeEnum::Renderer3D :
		{
			Renderer3D::Init();
			break;
		}
		default:
			break;
		}
	}

}