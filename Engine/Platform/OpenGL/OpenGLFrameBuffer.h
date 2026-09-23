#pragma once

#include <glad/glad.h>
#include <vector>
#include "Renderer/FrameBuffer.h"

namespace Monsi {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		~OpenGLFrameBuffer() override;

		void InvalidateFrameBuffer();

		void Resize(uint32_t width, uint32_t height) override;

		void Bind() override;
		void Unbind() override;

		void BlitToWindow() override;

		FrameBufferSpec& GetSpecification() override { return m_Specification; }
		const FrameBufferSpec& GetSpecification() const override { return m_Specification; }

		uint32_t GetColorAttachmentID(uint32_t index = 0) const override
		{
			ENGINE_ASSERT(index < m_ColorAttachments.size(), "Attachment index out of bounds!");
			return m_ColorAttachments[index];
		}
		uint32_t GetDepthAttachmentID() const override { return m_Depth; }

	private:
		GLuint GenerateColorAttachmentDSA(FrameBufferTextureFormat format, uint32_t width, uint32_t height);
		GLuint GenerateDepthBufferDSA(uint32_t width, uint32_t height);
		void Cleanup();

	private:
		uint32_t m_ID = 0;
		std::vector<uint32_t> m_ColorAttachments;
		std::vector<FrameBufferTextureFormat> m_ColorAttachmentFormats;
		uint32_t m_Depth = 0;
		FrameBufferSpec m_Specification;
	};

}