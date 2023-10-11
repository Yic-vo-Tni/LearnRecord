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

#define BIT(x) (1 << x)

#endif //LEARN_RECORD_CORE_H
