#pragma once

#include "Core.h"
#include <vector>

namespace Monsi {

	enum class FrameBufferTextureFormat {
		None = 0,
		RGBA8,
		RGBA16F
	};

	struct FrameBufferSpec {
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;

		std::vector<FrameBufferTextureFormat> Attachments;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;

		virtual FrameBufferSpec& GetSpecification() = 0;
		virtual const FrameBufferSpec& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void BlitToWindow() = 0;

		uint32_t GetColorTextureID() const { return GetColorAttachmentID(0); }
		uint32_t GetNormalTextureID() const { return GetColorAttachmentID(1); }

		static Reference<FrameBuffer> Create(const FrameBufferSpec& spec);
	};
}