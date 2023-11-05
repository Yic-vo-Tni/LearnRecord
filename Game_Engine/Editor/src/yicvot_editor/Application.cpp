//
// Created by lenovo on 10/9/2023.
//

#include "Application.h"

#include "Log.h"
#include "glad/glad.h"
#include "input.h"

namespace miku {

#define bind_event_fn(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::instance_ = nullptr;

    Application::Application() {
        YIC_CORE_ASSERT(!instance_, "Application already exists!");
        instance_ = this;

        window_ = std::unique_ptr<Window>(Window::Create());
        window_->setEventCallback(bind_event_fn(Application::onEvent));
    }

    Application::~Application() {


    }

    void Application::pushLayer(miku::Layer *layer) {
        layerStack_.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(miku::Layer *layer) {
        layerStack_.pushOverlay(layer);
        layer->onAttach();
    }

    void Application::onEvent(miku::Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(bind_event_fn(Application::onWindowClosed));

        YIC_CORE_TRANCE("{0}", e);

        for(auto item = layerStack_.end(); item != layerStack_.begin();){
            (*--item)->onEvent(e);
            if(e.handled_){
                break;
            }
        }
    }

    bool Application::onWindowClosed(miku::WindowCloseEvent &e) {
        running_ = false;

        return true;
    }

    void Application::run() {
        while (running_){
            glClearColor(1.f, 0.f, 1.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            for(Layer* layer : layerStack_){
                layer->onUpdate();
            }
            auto[x, y] = Input::getMousePosition();
            //YIC_CORE_TRANCE("{0}, {1}", x, y);

            window_->onUpdate();
        }
    }
}