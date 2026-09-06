#pragma once

namespace Monsi {

	enum class RenderTypeEnum {
		RendererNull,
		Renderer2D,
		Renderer3D
	};

	class RenderSystem {
	public:
		static void Init(RenderTypeEnum type);
		static void Shutdown();

		inline static RenderTypeEnum GetActiveType() { return s_ActiveType; }

	private:
		static RenderTypeEnum s_ActiveType;
	};

}