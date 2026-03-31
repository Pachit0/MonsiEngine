#pragma once

#include "core.h"

namespace Monsi {

	struct FrameBufferSpec {
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer {

	public:
		virtual ~FrameBuffer() = default;

		virtual FrameBufferSpec& GetSpecification() = 0;
		virtual const FrameBufferSpec& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentID() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Reference<FrameBuffer> Create(const FrameBufferSpec& spec);
	};
}