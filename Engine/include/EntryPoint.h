#pragma once
#include "Engine.h"
#include <iostream>

#ifdef _WIN32
    extern std::unique_ptr<Monsi::Application> Monsi::CreateApplication();

    int main() {
        std::cout << "Entry point of Monsi!" << std::endl;
        auto app = Monsi::CreateApplication();
        app->Run();
    }
#endif
