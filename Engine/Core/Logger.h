#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Monsi {
	class ENGINE_API Log {
	public:
		
		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		
	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define ENGINE_LOG_TRACE(...) ::Monsi::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_LOG_INFO(...)  ::Monsi::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_LOG_WARN(...)  ::Monsi::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_LOG_ERROR(...) ::Monsi::Log::GetEngineLogger()->error(__VA_ARGS__)

#define CLIENT_LOG_TRACE(...) ::Monsi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_LOG_INFO(...)  ::Monsi::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_LOG_WARN(...)  ::Monsi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_LOG_ERROR(...) ::Monsi::Log::GetClientLogger()->error(__VA_ARGS__)

