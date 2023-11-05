//
// Created by lenovo on 10/9/2023.
//

#ifndef LEARN_RECORD_CORE_H
#define LEARN_RECORD_CORE_H

#ifdef YIC_PLATFORM_WINDOWS
    #ifdef YIC_BUILD_DLL
        #define YIC_API __declspec(dllexport)
#else
        #define YIC_API __declspec(dllimport)
    #endif
#else
    #error Yicvot only support Windows
#endif

#ifdef YIC_ENABLE_ASSERTS
    #define YIC_ASSERT(x, ...) {if(!(x)) {YIC_ERROR("Assertion failed :{0}", __VA_ARGS__); debugbreak();}}
    #define YIC_CORE_ASSERT(x, ...) { if(!(x)) { YIC_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); debugbreak();}}
#else
    #define YIC_ASSERT(x, ...)
    #define YIC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define yic_bind_event_fn(fn) std::bind(&fn, this, std::placeholders::_1)

#endif //LEARN_RECORD_CORE_H
