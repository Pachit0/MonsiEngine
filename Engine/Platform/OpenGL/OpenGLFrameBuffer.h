#pragma once

#include "Renderer/FrameBuffer.h"

namespace Monsi {

	class OpenGLFrameBuffer : public FrameBuffer {

	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		virtual ~OpenGLFrameBuffer();

		void InvalidateFrameBuffer();

		void Bind() override;
		void Unbind() override;

		FrameBufferSpec& GetSpecification() override { return m_Specification; }
		const FrameBufferSpec& GetSpecification() const override { return m_Specification; }

		uint32_t GetColorAttachmentID() const override { return m_Color; }
	private:
		uint32_t m_ID;
		uint32_t m_Color, m_Depth;
		FrameBufferSpec m_Specification;
	};

}