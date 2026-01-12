#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Animation/AnimationClip.h"
#include <unordered_map>
#include <string>

namespace Engine {

    // Animation state with transition support
    struct AnimationState {
        std::string Name;
        Ref<AnimationClip> Clip;
        float Speed = 1.0f;
        
        AnimationState() = default;
        AnimationState(const std::string& name, const Ref<AnimationClip>& clip, float speed = 1.0f)
            : Name(name), Clip(clip), Speed(speed) {}
    };

    // Animator - manages animation playback and state transitions
    class Animator {
    public:
        Animator() = default;

        // Add animation clip
        void AddClip(const std::string& name, const Ref<AnimationClip>& clip, float speed = 1.0f);
        
        // Remove animation clip
        void RemoveClip(const std::string& name);
        
        // Play animation
        void Play(const std::string& name, bool forceRestart = false);
        
        // Stop current animation
        void Stop();
        
        // Pause/Resume
        void Pause() { m_Paused = true; }
        void Resume() { m_Paused = false; }
        bool IsPaused() const { return m_Paused; }
        
        // Update animation
        void Update(float deltaTime);
        
        // Get current frame
        const AnimationFrame* GetCurrentFrame() const;
        Ref<SubTexture2D> GetCurrentTexture() const;
        
        // State queries
        const std::string& GetCurrentAnimationName() const { return m_CurrentAnimation; }
        bool IsPlaying() const { return !m_CurrentAnimation.empty() && !m_Paused; }
        bool HasAnimation(const std::string& name) const;
        
        // Time control
        float GetCurrentTime() const { return m_CurrentTime; }
        void SetCurrentTime(float time) { m_CurrentTime = time; }
        
        float GetSpeed() const { return m_Speed; }
        void SetSpeed(float speed) { m_Speed = speed; }
        
        // Get all animation names
        std::vector<std::string> GetAnimationNames() const;

    private:
        std::unordered_map<std::string, AnimationState> m_Animations;
        std::string m_CurrentAnimation;
        float m_CurrentTime = 0.0f;
        float m_Speed = 1.0f;
        bool m_Paused = false;
    };

} // namespace Engine

