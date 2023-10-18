//
// Created by lenovo on 10/13/2023.
//

#include "yic.h"

namespace yic{
    void init(createSurfaceFunc func, int w, int h){
        Context::init(func);
        Context::getInstance().initSwapChain(w, h);
    }

    void quit(){
        Context::getInstance().destroySwapChain();
        Context::quit();
    }
}