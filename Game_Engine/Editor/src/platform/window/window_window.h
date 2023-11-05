//
// Created by lenovo on 10/21/2023.
//

#ifndef LEARN_RECORD_WINDOW_WINDOW_H
#define LEARN_RECORD_WINDOW_WINDOW_H

#include "yic_pch.h"
#include "yicvot_editor/window.h"


struct GLFWwindow{};

namespace miku{

    class WindowsWindow : public Window{
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow() override;

        void onUpdate() override;

        inline unsigned int getWidth() const override { return data_window.width; }
        inline unsigned int getHeight() const override { return data_window.height; }

        inline void setEventCallback(const EventCallbackFn& callback) override { data_window.eventCallback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

        inline virtual void* getNativeWindow() const { return window; };

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

        GLFWwindow* window;

        struct windowData{
            std::string name;
            unsigned int width, height;
            bool verticalSync;

            EventCallbackFn eventCallback;
        };

        windowData data_window;
    };


} // yic

#endif //LEARN_RECORD_WINDOW_WINDOW_H
