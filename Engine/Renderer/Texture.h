#pragma once

#include "Core.h"
#include <string>

namespace Monsi {

	class Texture {
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void modifyData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Reference<Texture2D> Create(const std::string& ResourcePath);
		static Reference<Texture2D> Create(uint32_t width, uint32_t height);
	};
}