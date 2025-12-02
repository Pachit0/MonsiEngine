#pragma once
#include "Engine.h"

#ifdef MONSI_PLATFORM_WINDOWS

    extern Monsi::Application* Monsi::CreateApplication();

    int main(int agrc, char* argv[]) {
        Monsi::Log::Init();

        auto app = Monsi::CreateApplication();
        app->Run();
        delete app;
    }

#endif
