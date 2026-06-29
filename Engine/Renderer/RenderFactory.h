#pragma once

namespace Monsi {

	enum class RenderTypeEnum {

		RendererNull, Renderer2D, Renderer3D

	};

	class RenderFactory {
	
	public:
		static void InitRenderType(RenderTypeEnum type);
	};

}