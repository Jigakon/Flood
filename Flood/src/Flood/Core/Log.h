#ifndef LOG__H
#define LOG__H

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Flood
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return s_AppLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_EngLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;

	};
}

#define FLOOD_ENGINE_TRACE(...)		::Flood::Log::GetEngineLogger()->trace(__VA_ARGS__);
#define FLOOD_ENGINE_ERROR(...)		::Flood::Log::GetEngineLogger()->error(__VA_ARGS__);
#define FLOOD_ENGINE_WARN(...)		::Flood::Log::GetEngineLogger()->warn(__VA_ARGS__);
#define FLOOD_ENGINE_INFO(...)		::Flood::Log::GetEngineLogger()->info(__VA_ARGS__);
#define FLOOD_ENGINE_CRITICAL(...)		::Flood::Log::GetEngineLogger()->critical(__VA_ARGS__);

#define FLOOD_APP_TRACE(...)		::Flood::Log::GetApplicationLogger()->trace(__VA_ARGS__);
#define FLOOD_APP_ERROR(...)		::Flood::Log::GetApplicationLogger()->error(__VA_ARGS__);
#define FLOOD_APP_WARN(...)			::Flood::Log::GetApplicationLogger()->warn(__VA_ARGS__);
#define FLOOD_APP_INFO(...)			::Flood::Log::GetApplicationLogger()->info(__VA_ARGS__);
#define FLOOD_APP_CRITICAL(...)		::Flood::Log::GetApplicationLogger()->critical(__VA_ARGS__);

#endif //!LOG__H
