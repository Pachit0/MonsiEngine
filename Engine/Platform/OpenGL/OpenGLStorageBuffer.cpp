#include "MonsiPch.h"
#include "OpenGLStorageBuffer.h"

#include "glad/glad.h"

namespace Monsi {

	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size) {
		ENGINE_PROFILER_FUNCTION();
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer() {
		ENGINE_PROFILER_FUNCTION();
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLStorageBuffer::Bind(uint32_t bindingPoint) const {
		ENGINE_PROFILER_FUNCTION();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_ID);
	}

	void OpenGLStorageBuffer::Unbind() const {
		ENGINE_PROFILER_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLStorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_ID, offset, size, data);
	}

}