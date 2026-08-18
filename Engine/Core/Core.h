#pragma once
#include <memory>

#ifdef _WIN32
    #ifdef _WIN64
        #define MONSI_PLATFORM_WINDOWS
    #else
        #error "x86 architecture not supported!"
    #endif
#elif defined(__linux__)
    #define MONSI_PLATFORM_LINUX
#endif

#ifdef MONSI_BUILD_DEBUG
    #define MONSI_ENABLE_ASSERTS
#endif

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__x86_64__) || defined(__i386__)
        #define DEBUG_BREAK() __asm__ volatile("int $3")
    #elif defined(__aarch64__) || defined(__arm__)
        #define DEBUG_BREAK() __asm__ volatile("brk #0")
    #else
        #include <signal.h>
        #define DEBUG_BREAK() raise(SIGTRAP)
    #endif
#else
    #define DEBUG_BREAK()
#endif

#ifdef MONSI_ENABLE_ASSERTS
    #define CLIENT_ASSERT(x, ...) if(!(x)) { CLIENT_LOG_ERROR("Assert fail: {0}", __VA_ARGS__); DEBUG_BREAK(); }
    #define ENGINE_ASSERT(x, ...) if(!(x)) { ENGINE_LOG_ERROR("Assert fail: {0}", __VA_ARGS__); DEBUG_BREAK(); }
#else
    #define CLIENT_ASSERT(x, ...)
    #define ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...); }
#define ENGINE_CONCAT_INTERNAL(x, y) x##y
#define ENGINE_CONCAT(x, y) ENGINE_CONCAT_INTERNAL(x, y)

namespace Monsi{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Reference = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Reference<T> CreateReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Weak_Reference = std::weak_ptr<T>;
	template<typename T>
	constexpr Weak_Reference<T> CreateWeakReference(const Reference<T>& sharedRef)
	{
		return Weak_Reference<T>(sharedRef);
	}
}

#ifdef MONSI_PLATFORM_WINDOWS
    #if ENGINE_LINK_DYNAMICALLY
        #ifdef ENGINE_BUILD_DLL
            #define ENGINE_API __declspec(dllexport)
        #else
            #define ENGINE_API __declspec(dllimport)
        #endif
    #else
        #define ENGINE_API
    #endif
#else
	#define ENGINE_API
#endif

#ifdef MONSI_PLATFORM_LINUX // placeholder
    #if ENGINE_LINK_DYNAMICALLY
        #ifdef ENGINE_BUILD_DLL
            #define ENGINE_API __declspec(dllexport)
        #else
            #define ENGINE_API __declspec(dllimport)
        #endif
    #else
        #define ENGINE_API
    #endif
#else
	#define ENGINE_API
#endif