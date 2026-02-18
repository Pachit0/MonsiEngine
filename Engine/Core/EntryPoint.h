#pragma once
#include "Core.h"
#include "debug/instrumentor.h"

#ifdef MONSI_PLATFORM_WINDOWS

    extern Monsi::Application* Monsi::CreateApplication();

    int main(int agrc, char* argv[]) {
        Monsi::Log::Init();

        ENGINE_PROFILER_BEGIN("StartUp", "Monsi_StartUp.json");
        auto app = Monsi::CreateApplication();
        ENGINE_PROFILER_END();

        ENGINE_PROFILER_BEGIN("RunTime", "Monsi_RunTime.json");
        app->Run();
        ENGINE_PROFILER_END();

        ENGINE_PROFILER_BEGIN("ShutDwon", "Monsi_ShutDown.json");
        delete app;
        ENGINE_PROFILER_END();
    }

#endif
