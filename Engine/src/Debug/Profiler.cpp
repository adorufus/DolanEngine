#include "Engine/Debug/Profiler.h"

namespace Engine {

    Profiler& Profiler::Get() {
        static Profiler instance;
        return instance;
    }

    void Profiler::BeginSession(const std::string& name) {
        m_Results.clear();
        m_ProfileStack.clear();
        m_CurrentDepth = 0;
    }

    void Profiler::EndSession() {
        // Finalize any remaining profiles (shouldn't happen)
        while (!m_ProfileStack.empty()) {
            EndProfile();
        }
    }

    void Profiler::BeginProfile(const std::string& name) {
        ProfileEntry entry;
        entry.Name = name;
        entry.Start = std::chrono::high_resolution_clock::now();
        entry.Depth = m_CurrentDepth;
        
        m_ProfileStack.push_back(entry);
        m_CurrentDepth++;
    }

    void Profiler::EndProfile() {
        if (m_ProfileStack.empty())
            return;

        auto entry = m_ProfileStack.back();
        m_ProfileStack.pop_back();
        m_CurrentDepth--;

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - entry.Start).count();

        ProfileResult result;
        result.Name = entry.Name;
        result.Duration = duration / 1000.0f; // Convert to milliseconds
        result.Depth = entry.Depth;

        m_Results.push_back(result);
    }

} // namespace Engine

