//
// Created by lenovo on 10/13/2023.
//

#ifndef LEARN_RECORD_APPLICATION_H
#define LEARN_RECORD_APPLICATION_H

#include "../throw/shader.h"
#include "../throw/pipeline.h"

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
        //Shader shader{R"(F:\Program\LearningRecord\Api_and_Library_Learn\Vulkan_C++_Style\shader\vertexShader.vert.spv)", "F:\\Program\\LearningRecord\\Api_and_Library_Learn\\Vulkan_C++_Style\\shader\\fragmentShader.frag.spv", device};
        //Pipeline pipeline{width, height, shader, device};
        //SwapChain swapChain{width, height, device, window};
       // std::unique_ptr<SwapChain> swapChain = std::make_unique<SwapChain>(device);
        //Pipeline pipeline{width, height, shader, device, swapChain};

    };

}

#endif //LEARN_RECORD_APPLICATION_H
