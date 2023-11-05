//
// Created by lenovo on 10/21/2023.
//

#ifndef LEARN_RECORD_WINDOW_H
#define LEARN_RECORD_WINDOW_H

#include <utility>

#include "yic_pch.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"

namespace miku{
    struct WindowProps{
        std::string Title;
        unsigned int Width, Height;

        WindowProps(std::string  title = "yic Engine", unsigned int width = 1280, unsigned int height = 720) : Title(std::move(title)), Width(width), Height(height){

        }
    };

    class YIC_API Window{
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    private:
    };
}

#endif //LEARN_RECORD_WINDOW_H
