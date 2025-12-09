#pragma once

#include "Core.h"
#include "Events.h"
#include "ApplicationEvent.h"

#include "Window.h"

namespace Monsi {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& event);

    private:
        bool OnWindowClose(WindowCloseEvent& event);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    Application* CreateApplication();

}