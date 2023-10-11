//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_ENTRYPOINT_H
#define LEARN_RECORD_ENTRYPOINT_H

#ifdef YIC_PLATFORM_WINDOWS

    extern miku::Application* miku::createApplication();

    int main(){
        miku::Log::init();
        YIC_CORE_WARN("initialized log");
        int a = 5;
        YIC_INFO("hello var = {0}", a);


        auto app = miku::createApplication();
        app->run();
        delete app;
    }

#endif

#endif //LEARN_RECORD_ENTRYPOINT_H
