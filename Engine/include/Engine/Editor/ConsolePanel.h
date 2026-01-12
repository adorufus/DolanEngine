#pragma once

#include "Engine/Core/Base.h"
#include <vector>
#include <string>

namespace Engine {

    enum class LogLevel {
        Trace = 0,
        Info,
        Warning,
        Error
    };

    struct LogMessage {
        LogLevel Level;
        std::string Message;
        std::string Timestamp;
    };

    class ConsolePanel {
    public:
        ConsolePanel();
        ~ConsolePanel() = default;

        void OnImGuiRender();
        void Clear();
        
        static void AddLog(LogLevel level, const std::string& message);

    private:
        static std::vector<LogMessage> s_Messages;
        static const int s_MaxMessages = 1000;
        
        bool m_AutoScroll = true;
        bool m_ShowTrace = true;
        bool m_ShowInfo = true;
        bool m_ShowWarning = true;
        bool m_ShowError = true;
    };

} // namespace Engine

