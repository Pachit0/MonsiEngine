#pragma once

#include "Core.h"

namespace Monsi {

	class StorageBuffer {
	public:
		virtual ~StorageBuffer() = default;

		virtual void Bind(uint32_t bindingPoint = 0) const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Reference<StorageBuffer> Create(uint32_t size);
	};

}