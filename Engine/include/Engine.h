#pragma once

#include "Core.h"
#include "Events.h"

namespace Monsi {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
            
    };

    Application* CreateApplication();

}