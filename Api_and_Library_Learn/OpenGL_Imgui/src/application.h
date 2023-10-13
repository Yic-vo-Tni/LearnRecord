//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_APPLICATION_H
#define LEARN_RECORD_APPLICATION_H

#include "render.h"
#include "shader.h"

#include "memory"

namespace yic{

    class Application{
    public:
        static constexpr int width = 1200;
        static constexpr int height = 900;

        Application() = default;
        ~Application() = default;
        Application(const Application&) = delete;
        Application &operator = (const Application&) = delete;

        void run();

    private:
        Window yic_window{width, height, "yicvot"};
        Render render{width, height, yic_window};



        std::string text{"Text"};
        char textbox[50]{"text box"};
        ImVec4 color{};



    };
}

#endif //LEARN_RECORD_APPLICATION_H
