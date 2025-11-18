#pragma once

#include "Core.h"
#include <memory>

namespace Monsi {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void Print();

    };

    std::unique_ptr<Application> CreateApplication();

}