#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include <vector>
#include <string>

namespace Engine {

    // Single frame in an animation
    struct AnimationFrame {
        Ref<SubTexture2D> Texture;
        float Duration = 0.1f; // Duration in seconds
        
        AnimationFrame() = default;
        AnimationFrame(const Ref<SubTexture2D>& texture, float duration = 0.1f)
            : Texture(texture), Duration(duration) {}
    };

    // Animation clip - a sequence of frames
    class AnimationClip {
    public:
        AnimationClip() = default;
        AnimationClip(const std::string& name, bool loop = true);

        // Add frame to the animation
        void AddFrame(const Ref<SubTexture2D>& texture, float duration = 0.1f);
        void AddFrame(const AnimationFrame& frame);
        
        // Get frame at specific index
        const AnimationFrame& GetFrame(size_t index) const;
        
        // Get frame at specific time
        const AnimationFrame& GetFrameAtTime(float time) const;
        
        // Get frame index at specific time
        size_t GetFrameIndexAtTime(float time) const;
        
        // Get total duration of animation
        float GetDuration() const { return m_TotalDuration; }
        
        // Get number of frames
        size_t GetFrameCount() const { return m_Frames.size(); }
        
        // Properties
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }
        
        bool IsLooping() const { return m_Loop; }
        void SetLooping(bool loop) { m_Loop = loop; }
        
        float GetFrameRate() const { return m_FrameRate; }
        void SetFrameRate(float fps);
        
        // Utility - Create from sprite sheet
        static Ref<AnimationClip> CreateFromSpriteSheet(
            const std::string& name,
            const Ref<Texture2D>& spriteSheet,
            const glm::vec2& startCoord,
            const glm::vec2& cellSize,
            size_t frameCount,
            float frameDuration = 0.1f,
            bool loop = true
        );

    private:
        void RecalculateDuration();

    private:
        std::string m_Name;
        std::vector<AnimationFrame> m_Frames;
        float m_TotalDuration = 0.0f;
        float m_FrameRate = 10.0f;
        bool m_Loop = true;
    };

} // namespace Engine

