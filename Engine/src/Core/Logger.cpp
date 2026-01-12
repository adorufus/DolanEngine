#include "Engine/Core/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {

    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Init() {
        // Set pattern: [timestamp] [logger name] [level] message
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        // Create core logger
        s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        // Create client logger
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        
        GE_CORE_INFO("Logger initialized");
    }

}

