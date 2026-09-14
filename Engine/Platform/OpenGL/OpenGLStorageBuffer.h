#pragma once

#include "StorageBuffer.h"

namespace Monsi {

	class OpenGLStorageBuffer : public StorageBuffer {
	public:
		OpenGLStorageBuffer(uint32_t size);
		virtual ~OpenGLStorageBuffer();

		void Bind(uint32_t bindingPoint = 0) const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_ID = 0;
	};

}