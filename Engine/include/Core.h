#pragma once
#include <memory>

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
    #error Monsi supports only Windows!
#endif

#ifdef MONSI_BUILD_DEBUG
    #define MONSI_ENABLE_ASSERTS
#endif

#ifdef MONSI_ENABLE_ASSERTS
    #define CLIENT_ASSERT(x, ...) if(!(x)) { CLIENT_LOG_ERROR("Assert fail: {0}", __VA_ARGS__); __debugbreak(); }
    #define ENGINE_ASSERT(x, ...) if(!(x)) { ENGINE_LOG_ERROR("Assert fail: {0}", __VA_ARGS__); __debugbreak(); }
#else
    #define CLIENT_ASSERT(x, ...)
    #define ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ENGINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Monsi{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Reference = std::shared_ptr<T>;
}