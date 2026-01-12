#include "Engine/Editor/ProfilerPanel.h"
#include "Engine/Debug/Profiler.h"
#include <imgui.h>
#include <numeric>

namespace Engine {

    ProfilerPanel::ProfilerPanel() {
        m_FrameTimeHistory.reserve(s_MaxHistory);
    }

    void ProfilerPanel::OnImGuiRender() {
        ImGui::Begin("Performance Profiler");

        auto& profiler = Profiler::Get();
        float frameTime = profiler.GetFrameTime();

        // Update history
        m_FrameTimeHistory.push_back(frameTime);
        if (m_FrameTimeHistory.size() > s_MaxHistory) {
            m_FrameTimeHistory.erase(m_FrameTimeHistory.begin());
        }

        // Calculate average FPS
        if (!m_FrameTimeHistory.empty()) {
            float avgFrameTime = std::accumulate(m_FrameTimeHistory.begin(), m_FrameTimeHistory.end(), 0.0f) / m_FrameTimeHistory.size();
            m_AverageFPS = 1000.0f / avgFrameTime; // Convert ms to FPS
        }

        // Current stats
        ImGui::Text("Frame Time: %.3f ms", frameTime);
        ImGui::Text("FPS: %.1f (avg: %.1f)", 1000.0f / frameTime, m_AverageFPS);
        ImGui::Separator();

        // Controls
        ImGui::Checkbox("Show Graph", &m_ShowGraph);
        ImGui::SameLine();
        ImGui::Checkbox("Show Details", &m_ShowDetails);
        ImGui::SameLine();
        if (ImGui::Button("Clear History")) {
            Clear();
        }

        // Frame time graph
        if (m_ShowGraph && !m_FrameTimeHistory.empty()) {
            ImGui::Separator();
            ImGui::Text("Frame Time History (120 frames)");
            
            // Find min/max for scaling
            float minTime = *std::min_element(m_FrameTimeHistory.begin(), m_FrameTimeHistory.end());
            float maxTime = *std::max_element(m_FrameTimeHistory.begin(), m_FrameTimeHistory.end());
            
            char overlay[32];
            sprintf(overlay, "%.1f ms (max)", maxTime);
            ImGui::PlotLines("##frametime", m_FrameTimeHistory.data(), m_FrameTimeHistory.size(),
                           0, overlay, 0.0f, maxTime * 1.2f, ImVec2(0, 80));
        }

        // Detailed profile results
        if (m_ShowDetails) {
            ImGui::Separator();
            ImGui::Text("Profile Results:");
            
            auto& results = profiler.GetResults();
            if (results.empty()) {
                ImGui::TextDisabled("No profiling data available");
                ImGui::TextDisabled("Enable profiling with GE_ENABLE_PROFILING");
            } else {
                ImGui::BeginChild("ProfileResults", ImVec2(0, 200), true);
                
                for (const auto& result : results) {
                    // Indent based on depth
                    for (size_t i = 0; i < result.Depth; ++i) {
                        ImGui::Indent(10.0f);
                    }
                    
                    // Color code by duration
                    ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
                    if (result.Duration > 5.0f) {
                        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
                    }
                    if (result.Duration > 10.0f) {
                        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
                    }
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::Text("%s: %.3f ms", result.Name.c_str(), result.Duration);
                    ImGui::PopStyleColor();
                    
                    // Unindent
                    for (size_t i = 0; i < result.Depth; ++i) {
                        ImGui::Unindent(10.0f);
                    }
                }
                
                ImGui::EndChild();
            }
        }

        // Performance tips
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Performance Tips:");
        ImGui::BulletText("Target: 16.67 ms (60 FPS) or 8.33 ms (120 FPS)");
        ImGui::BulletText("Yellow: >5ms, Red: >10ms");
        ImGui::BulletText("Optimize red functions first");

        ImGui::End();
    }

    void ProfilerPanel::Clear() {
        m_FrameTimeHistory.clear();
        Profiler::Get().ClearResults();
    }

} // namespace Engine

