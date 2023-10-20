//
// Created by lenovo on 10/13/2023.
//

#ifndef LEARN_RECORD_APPLICATION_H
#define LEARN_RECORD_APPLICATION_H

#include "renderer.h"

namespace yic {

    class Application {
    public:
        Application() = default;
        ~Application() = default;

        Application(const Application&) = delete;
        Application& operator = (const Application&) = delete;

        void run();

    private:
        const int width = 800;
        const int height = 600;
        Window window{width, height, "yic"};
        Device device{window};
        SwapChain swapChain{window, device};
        Pipeline pipeline{device, swapChain};
        Renderer renderer{device, swapChain, pipeline};

    };

}

#endif //LEARN_RECORD_APPLICATION_H
