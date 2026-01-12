#pragma once

#include "Engine/Core/Base.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Engine {

    class Logger {
    public:
        static void Init();
        
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
        
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Core logging macros
#define GE_CORE_TRACE(...)    ::Engine::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::Engine::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::Engine::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::Engine::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_CRITICAL(...) ::Engine::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging macros
#define GE_TRACE(...)         ::Engine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)          ::Engine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)          ::Engine::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)         ::Engine::Logger::GetClientLogger()->error(__VA_ARGS__)
#define GE_CRITICAL(...)      ::Engine::Logger::GetClientLogger()->critical(__VA_ARGS__)

