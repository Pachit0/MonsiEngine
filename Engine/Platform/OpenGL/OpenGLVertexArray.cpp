#include "MonsiPch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Monsi {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:  return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3:   return GL_FLOAT;
		case ShaderDataType::Mat4:   return GL_FLOAT;

		case ShaderDataType::Int:    return GL_INT;
		case ShaderDataType::Int2:   return GL_INT;
		case ShaderDataType::Int3:   return GL_INT;
		case ShaderDataType::Int4:   return GL_INT;

		case ShaderDataType::Bool:   return GL_UNSIGNED_BYTE;
		}

		ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return GL_NONE;
	}

	static bool IsIntegerShaderDataType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
			return true;

		default:
			return false;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ENGINE_PROFILER_FUNCTION();
		glCreateVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ENGINE_PROFILER_FUNCTION();
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ENGINE_PROFILER_FUNCTION();
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		ENGINE_PROFILER_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer)
	{
		ENGINE_PROFILER_FUNCTION();

		ENGINE_ASSERT(
			vertexBuffer->GetLayout().GetElements().size(),
			"Vertex Buffer doesn't have a layout!");

		glBindVertexArray(m_ID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4)
			{
				const uint32_t columnCount =
					element.Type == ShaderDataType::Mat3 ? 3 : 4;

				const uint32_t columnSize = sizeof(float) * columnCount;

				for (uint32_t column = 0; column < columnCount; ++column)
				{
					glEnableVertexAttribArray(m_VBIndex);

					glVertexAttribPointer(
						m_VBIndex,
						columnCount,
						GL_FLOAT,
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						reinterpret_cast<const void*>(
							static_cast<uintptr_t>(element.Offset + columnSize * column)));

					glVertexAttribDivisor(
						m_VBIndex,
						element.IsInstanced ? 1 : 0);

					++m_VBIndex;
				}

				continue;
			}

			glEnableVertexAttribArray(m_VBIndex);

			if (IsIntegerShaderDataType(element.Type))
			{
				glVertexAttribIPointer(
					m_VBIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					reinterpret_cast<const void*>(
						static_cast<uintptr_t>(element.Offset)));
			}
			else
			{
				glVertexAttribPointer(
					m_VBIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					reinterpret_cast<const void*>(
						static_cast<uintptr_t>(element.Offset)));
			}

			glVertexAttribDivisor(
				m_VBIndex,
				element.IsInstanced ? 1 : 0);

			++m_VBIndex;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer)
	{
		ENGINE_PROFILER_FUNCTION();

		glBindVertexArray(m_ID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}