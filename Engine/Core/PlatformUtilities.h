#pragma once

#include <string>

namespace Monsi {

	class FileDialogs {
	public:
		static std::string OpenFile(const char* file);
		static std::string SaveFile(const char* file);
	};

}