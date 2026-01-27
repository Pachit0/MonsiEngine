#include "MonsiPch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Monsi {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case Monsi::ShaderDataType::Float:  return GL_FLOAT;
		case Monsi::ShaderDataType::Float2: return GL_FLOAT;
		case Monsi::ShaderDataType::Float3: return GL_FLOAT;
		case Monsi::ShaderDataType::Float4: return GL_FLOAT;
		case Monsi::ShaderDataType::Mat3:   return GL_FLOAT;
		case Monsi::ShaderDataType::Mat4:   return GL_FLOAT;
		case Monsi::ShaderDataType::Int:    return GL_INT;
		case Monsi::ShaderDataType::Int2:   return GL_INT;
		case Monsi::ShaderDataType::Int3:   return GL_INT;
		case Monsi::ShaderDataType::Int4:   return GL_INT;
		case Monsi::ShaderDataType::Bool:   return GL_BOOL;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer doesn't have a layout!");
		glBindVertexArray(m_ID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(m_VBIndex);
			glVertexAttribPointer(m_VBIndex, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), (const void*)element.Offset);
			m_VBIndex++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_ID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}