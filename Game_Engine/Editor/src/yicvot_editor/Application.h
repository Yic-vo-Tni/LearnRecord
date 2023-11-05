//
// Created by lenovo on 10/9/2023.
//

#ifndef LEARN_RECORD_APPLICATION_H
#define LEARN_RECORD_APPLICATION_H

#include "yic_pch.h"
#include "Core.h"
#include "window.h"
#include "layerStack.h"

namespace miku {

    class YIC_API Application {
    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        inline static Application& get() { return *instance_; }
        inline Window& getWindow() { return *window_; }
    private:
        bool onWindowClosed(WindowCloseEvent& e);
        std::unique_ptr<Window> window_;
        bool running_ = true;
        LayerStack layerStack_;

        static Application* instance_;
    };

    Application* createApplication();


}


#endif //LEARN_RECORD_APPLICATION_H
