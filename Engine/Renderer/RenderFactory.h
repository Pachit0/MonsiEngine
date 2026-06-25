#pragma once

namespace Monsi {

	enum class RenderTypeEnum {

		Renderer2D, Renderer3D

	};

	class RenderFactory {
	
	public:
		static void InitRenderType(RenderTypeEnum type);
	};

}