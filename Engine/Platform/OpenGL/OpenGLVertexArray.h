#pragma once

#include "Renderer/VertexArray.h"

namespace Monsi {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) override;

		const std::vector<Reference<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Reference<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_ID;
		uint32_t m_VBIndex = 0;
		Reference<IndexBuffer> m_IndexBuffer;
		std::vector<Reference<VertexBuffer>> m_VertexBuffers;
	};

}