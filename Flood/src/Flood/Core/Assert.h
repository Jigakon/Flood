#ifndef ASSERT__H
#define ASSERT__H

#ifdef FLOOD_ENABLE_ASSERT
	#define FLOOD_ASSERT(x, ...) { if(!(x)) {FLOOD_APP_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak();} }
	#define FLOOD_ENGINE_ASSERT(x, ...) { if(!(x)) {FLOOD_ENGINE_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak();} }
#else
	#define FLOOD_ASSERT(x, ...)
	#define FLOOD_ENGINE_ASSERT(x, ...)
#endif

#endif // !ASSERT__H
