//
// Created by lenovo on 10/13/2023.
//

#ifndef LEARN_RECORD_YIC_H
#define LEARN_RECORD_YIC_H

#include "vulkan/vulkan.hpp"
#include "context.h"

namespace yic{
    void init(createSurfaceFunc func, int w, int h);
    void quit();
}

#endif //LEARN_RECORD_YIC_H
