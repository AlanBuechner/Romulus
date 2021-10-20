#pragma once
#include <memory>
#include <cstdint>
#include "Log.h"

#define ENABLE_ASSERTS

#ifdef _WIN32 // check if on windows
	#ifdef _WIN64 // check if windows is 64 bit
		#define PLATFORM_WINDOWS
	#else
		#error "x86 Buillds are not sapported" // if not 64 bit throw error
	#endif // _WIN64
#elif defined(__APPLE__) || defined(__MACH__) // check if on apple product
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1 // check if on iphone vm
		#error "IOS simulator is not sapported"
	#elif TARGET_OS_IPHONE == 1 // check if on iphone
		#define PLATFORM_IOS
		#error "IOS is not sapported"
	#elif TARGET_OS_MAC == 1 // check if on mack
		#define PLATFORM_MACOS
		#error "MacOS is not sapported"
	#endif
#elif defined(__ANDROID__) // check if on android
	#define PLATFORM_ANDROID
	#error "Android is not sapported"
#elif defined(__linux__) // check if on linux
	#define PLATFORM_LINUX
	#error "Linux is not sapported"
#else
	#error "Unknown Platform!"
#endif

#ifdef PLATFORM_WINDOWS

#else
	#error Platform not saported
#endif

#ifdef ENABLE_ASSERTS // add back in when logging is implemented
	#define ASSERT(x, ...) {if(!(x)) {DEBUG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define CORE_ASSERT(x, ...) {if(!(x)) {CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
	#define CORE_ASSERT(x, ...)
#endif // ENABLE_ASSERTS


#define BIT(x) (1 << x)

namespace Engine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	using byte = unsigned char;
	
	using uint8  = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using int8  = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;
}