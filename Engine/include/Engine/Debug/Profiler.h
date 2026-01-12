#pragma once

#include "Engine/Core/Base.h"
#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>

namespace Engine {

    struct ProfileResult {
        std::string Name;
        float Duration; // in milliseconds
        size_t Depth;
    };

    class Profiler {
    public:
        static Profiler& Get();

        void BeginSession(const std::string& name);
        void EndSession();

        void BeginProfile(const std::string& name);
        void EndProfile();

        const std::vector<ProfileResult>& GetResults() const { return m_Results; }
        void ClearResults() { m_Results.clear(); }

        float GetFrameTime() const { return m_FrameTime; }
        void SetFrameTime(float frameTime) { m_FrameTime = frameTime; }

    private:
        Profiler() = default;

        struct ProfileEntry {
            std::string Name;
            std::chrono::time_point<std::chrono::high_resolution_clock> Start;
            size_t Depth;
        };

        std::vector<ProfileEntry> m_ProfileStack;
        std::vector<ProfileResult> m_Results;
        float m_FrameTime = 0.0f;
        size_t m_CurrentDepth = 0;
    };

    // RAII profile scope
    class ProfileScope {
    public:
        ProfileScope(const std::string& name)
            : m_Name(name) {
            Profiler::Get().BeginProfile(m_Name);
        }

        ~ProfileScope() {
            Profiler::Get().EndProfile();
        }

    private:
        std::string m_Name;
    };

} // namespace Engine

// Profiling macros
#ifdef GE_ENABLE_PROFILING
    #define GE_PROFILE_SCOPE(name) Engine::ProfileScope profileScope##__LINE__(name)
    #define GE_PROFILE_FUNCTION() GE_PROFILE_SCOPE(__FUNCTION__)
#else
    #define GE_PROFILE_SCOPE(name)
    #define GE_PROFILE_FUNCTION()
#endif

