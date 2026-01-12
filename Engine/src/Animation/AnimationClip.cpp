#include "Engine/Animation/AnimationClip.h"
#include "Engine/Core/Logger.h"

namespace Engine {

    AnimationClip::AnimationClip(const std::string& name, bool loop)
        : m_Name(name), m_Loop(loop)
    {
    }

    void AnimationClip::AddFrame(const Ref<SubTexture2D>& texture, float duration)
    {
        m_Frames.emplace_back(texture, duration);
        RecalculateDuration();
    }

    void AnimationClip::AddFrame(const AnimationFrame& frame)
    {
        m_Frames.push_back(frame);
        RecalculateDuration();
    }

    const AnimationFrame& AnimationClip::GetFrame(size_t index) const
    {
        GE_CORE_ASSERT(index < m_Frames.size(), "Animation frame index out of range!");
        return m_Frames[index];
    }

    const AnimationFrame& AnimationClip::GetFrameAtTime(float time) const
    {
        if (m_Frames.empty())
        {
            GE_CORE_ERROR("Animation clip '{0}' has no frames!", m_Name);
            static AnimationFrame emptyFrame;
            return emptyFrame;
        }

        if (m_Loop && m_TotalDuration > 0.0f)
        {
            // Wrap time for looping animations
            time = fmod(time, m_TotalDuration);
        }

        float currentTime = 0.0f;
        for (const auto& frame : m_Frames)
        {
            currentTime += frame.Duration;
            if (time < currentTime)
                return frame;
        }

        // Return last frame if time exceeds duration (non-looping)
        return m_Frames.back();
    }

    size_t AnimationClip::GetFrameIndexAtTime(float time) const
    {
        if (m_Frames.empty())
            return 0;

        if (m_Loop && m_TotalDuration > 0.0f)
        {
            time = fmod(time, m_TotalDuration);
        }

        float currentTime = 0.0f;
        for (size_t i = 0; i < m_Frames.size(); ++i)
        {
            currentTime += m_Frames[i].Duration;
            if (time < currentTime)
                return i;
        }

        return m_Frames.size() - 1;
    }

    void AnimationClip::SetFrameRate(float fps)
    {
        m_FrameRate = fps;
        float frameDuration = 1.0f / fps;
        
        for (auto& frame : m_Frames)
        {
            frame.Duration = frameDuration;
        }
        
        RecalculateDuration();
    }

    Ref<AnimationClip> AnimationClip::CreateFromSpriteSheet(
        const std::string& name,
        const Ref<Texture2D>& spriteSheet,
        const glm::vec2& startCoord,
        const glm::vec2& cellSize,
        size_t frameCount,
        float frameDuration,
        bool loop)
    {
        auto clip = CreateRef<AnimationClip>(name, loop);
        
        for (size_t i = 0; i < frameCount; ++i)
        {
            glm::vec2 coord = startCoord + glm::vec2(i, 0);
            auto subTexture = SubTexture2D::CreateFromCoords(
                spriteSheet,
                coord,
                cellSize,
                {1.0f, 1.0f}
            );
            
            clip->AddFrame(subTexture, frameDuration);
        }
        
        return clip;
    }

    void AnimationClip::RecalculateDuration()
    {
        m_TotalDuration = 0.0f;
        for (const auto& frame : m_Frames)
        {
            m_TotalDuration += frame.Duration;
        }
    }

} // namespace Engine

