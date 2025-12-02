#pragma once

#ifdef MONSI_PLATFORM_WINDOWS
    #ifdef ENGINE_BUILD_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #error Monsi supports only Windows!
#endif

#define BIT(x) (1 << x)