#include "MonsiPch.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Monsi {

	std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [thread %t] %v");

		auto engine_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../Engine/Latest.log", true);
		std::vector<spdlog::sink_ptr> sinks{ engine_sink, file_sink };

		s_EngineLogger = std::make_shared<spdlog::logger>("MONSI", sinks.begin(), sinks.end());
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("SANDBOX_APP", sinks.begin(), sinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}