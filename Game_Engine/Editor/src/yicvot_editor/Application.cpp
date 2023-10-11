//
// Created by lenovo on 10/9/2023.
//

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace miku {

    Application::Application() {

    }

    Application::~Application() {


    }

    void Application::run() {
        WindowResizeEvent event(1280, 720);
        YIC_TRANCE(event);

        while (true){

        }
    }
}