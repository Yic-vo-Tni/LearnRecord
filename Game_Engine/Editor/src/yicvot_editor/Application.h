//
// Created by lenovo on 10/9/2023.
//

#ifndef LEARN_RECORD_APPLICATION_H
#define LEARN_RECORD_APPLICATION_H

#include "Core.h"

namespace miku {

    class YIC_API Application {
    public:
        Application();
        virtual ~Application();

        void run();


    };

    Application* createApplication();


}


#endif //LEARN_RECORD_APPLICATION_H
