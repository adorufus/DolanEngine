#include "Engine/Editor/ConsolePanel.h"
#include <imgui.h>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Engine {

    std::vector<LogMessage> ConsolePanel::s_Messages;

    ConsolePanel::ConsolePanel() {
    }

    void ConsolePanel::OnImGuiRender() {
        ImGui::Begin("Console");

        // Toolbar
        if (ImGui::Button("Clear")) {
            Clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto Scroll", &m_AutoScroll);
        ImGui::SameLine();
        ImGui::Checkbox("Trace", &m_ShowTrace);
        ImGui::SameLine();
        ImGui::Checkbox("Info", &m_ShowInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warning", &m_ShowWarning);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &m_ShowError);

        ImGui::Separator();

        // Scrollable log region
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        
        for (const auto& msg : s_Messages) {
            // Filter by level
            if (msg.Level == LogLevel::Trace && !m_ShowTrace) continue;
            if (msg.Level == LogLevel::Info && !m_ShowInfo) continue;
            if (msg.Level == LogLevel::Warning && !m_ShowWarning) continue;
            if (msg.Level == LogLevel::Error && !m_ShowError) continue;

            // Color code by level
            ImVec4 color;
            const char* prefix;
            switch (msg.Level) {
                case LogLevel::Trace:
                    color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                    prefix = "[TRACE]";
                    break;
                case LogLevel::Info:
                    color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    prefix = "[INFO]";
                    break;
                case LogLevel::Warning:
                    color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                    prefix = "[WARN]";
                    break;
                case LogLevel::Error:
                    color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    prefix = "[ERROR]";
                    break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted((msg.Timestamp + " " + prefix + " " + msg.Message).c_str());
            ImGui::PopStyleColor();
        }

        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
    }

    void ConsolePanel::Clear() {
        s_Messages.clear();
    }

    void ConsolePanel::AddLog(LogLevel level, const std::string& message) {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();

        LogMessage msg;
        msg.Level = level;
        msg.Message = message;
        msg.Timestamp = ss.str();

        s_Messages.push_back(msg);

        // Limit messages
        if (s_Messages.size() > s_MaxMessages) {
            s_Messages.erase(s_Messages.begin());
        }
    }

} // namespace Engine

