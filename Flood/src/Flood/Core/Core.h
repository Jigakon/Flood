#ifndef CORE__H
#define CORE__H

#include <memory>

#include "Flood/Core/PlatformDetection.h"

#ifdef FLOOD_DEBUG
	#if defined(FLOOD_PLATFORM_WINDOWS)
		#define FLOOD_DEBUGBREAK() __debugbreak()
	#elif defined(FLOOD_PLATFORM_LINUX)
		#include <signal.h>
		#define FLOOD_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define FLOOD_ENABLE_ASSERT
#else
	#define FLOOD_DEBUGBREAK()
#endif

#define BIND_EVENT_FN(func, obj) std::bind(&func, obj, std::placeholders::_1)


namespace Flood
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Flood/Core/Assert.h"
#include "Flood/Core/Log.h"

#endif //!CORE__H
