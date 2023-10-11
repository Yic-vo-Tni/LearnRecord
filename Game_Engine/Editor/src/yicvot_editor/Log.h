//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_LOG_H
#define LEARN_RECORD_LOG_H

#include "memory"
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace miku {

    class YIC_API Log {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger> &getCoreLogger() { return coreLogger; }

        inline static std::shared_ptr<spdlog::logger> &getClientLogger() { return clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };


}

#define YIC_CORE_TRANCE(...)    ::miku::Log::getCoreLogger()->trace(__VA_ARGS__)
#define YIC_CORE_INFO(...)      ::miku::Log::getCoreLogger()->info(__VA_ARGS__)
#define YIC_CORE_WARN(...)      ::miku::Log::getCoreLogger()->warn(__VA_ARGS__)
#define YIC_CORE_ERROR(...)     ::miku::Log::getCoreLogger()->error(__VA_ARGS__)
#define YIC_CORE_FATAL(...)     ::miku::Log::getCoreLogger()->fatal(__VA_ARGS__)

#define YIC_TRANCE(...)    ::miku::Log::getClientLogger()->trace(__VA_ARGS__)
#define YIC_INFO(...)      ::miku::Log::getClientLogger()->info(__VA_ARGS__)
#define YIC_WARN(...)      ::miku::Log::getClientLogger()->warn(__VA_ARGS__)
#define YIC_ERROR(...)     ::miku::Log::getClientLogger()->error(__VA_ARGS__)
#define YIC_FATAL(...)     ::miku::Log::getClientLogger()->fatal(__VA_ARGS__)


#endif //LEARN_RECORD_LOG_H
