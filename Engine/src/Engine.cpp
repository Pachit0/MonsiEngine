#include "mopch.h"
#include "Engine.h"

#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

#include "Logger.h"

namespace Monsi {
    
    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::Run() {
        WindowResizeEvent a(1920, 1080);
        MouseEventMoved b(50, 60);
        KeyEventPressed c(51, 1);

        MO_TRACE(a);
        MO_TRACE(b);
        MO_ENGINE_TRACE(c);

        while (true);
    }

}
