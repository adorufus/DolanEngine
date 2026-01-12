#pragma once

#include "Engine/Core/Base.h"
#include <vector>

namespace Engine {

    class ProfilerPanel {
    public:
        ProfilerPanel();
        ~ProfilerPanel() = default;

        void OnImGuiRender();
        void Clear();

    private:
        std::vector<float> m_FrameTimeHistory;
        static const int s_MaxHistory = 120; // 2 seconds at 60 FPS
        
        bool m_ShowGraph = true;
        bool m_ShowDetails = true;
        float m_AverageFPS = 0.0f;
    };

} // namespace Engine

