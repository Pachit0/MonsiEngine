#pragma once

namespace Monsi {

	class RenderContext {
	public:
		virtual ~RenderContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
		virtual void EnableCursor() = 0;
		virtual void DisableCursor() = 0;

		virtual int GetCursorStatus() = 0;
	};

}