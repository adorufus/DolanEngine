#include "Engine/Animation/Animator.h"
#include "Engine/Core/Logger.h"

namespace Engine {

    void Animator::AddClip(const std::string& name, const Ref<AnimationClip>& clip, float speed)
    {
        if (!clip)
        {
            GE_CORE_WARN("Attempted to add null animation clip '{0}'", name);
            return;
        }
        
        m_Animations[name] = AnimationState(name, clip, speed);
        GE_CORE_TRACE("Added animation clip '{0}' with {1} frames", name, clip->GetFrameCount());
    }

    void Animator::RemoveClip(const std::string& name)
    {
        auto it = m_Animations.find(name);
        if (it != m_Animations.end())
        {
            if (m_CurrentAnimation == name)
            {
                Stop();
            }
            m_Animations.erase(it);
        }
    }

    void Animator::Play(const std::string& name, bool forceRestart)
    {
        if (!HasAnimation(name))
        {
            GE_CORE_WARN("Animation '{0}' not found in animator", name);
            return;
        }

        if (m_CurrentAnimation == name && !forceRestart)
        {
            // Already playing this animation, just resume if paused
            m_Paused = false;
            return;
        }

        m_CurrentAnimation = name;
        m_CurrentTime = 0.0f;
        m_Paused = false;
        
        GE_CORE_TRACE("Playing animation '{0}'", name);
    }

    void Animator::Stop()
    {
        m_CurrentAnimation.clear();
        m_CurrentTime = 0.0f;
        m_Paused = false;
    }

    void Animator::Update(float deltaTime)
    {
        if (m_CurrentAnimation.empty() || m_Paused)
            return;

        auto it = m_Animations.find(m_CurrentAnimation);
        if (it == m_Animations.end())
        {
            GE_CORE_WARN("Current animation '{0}' not found", m_CurrentAnimation);
            Stop();
            return;
        }

        const auto& state = it->second;
        const auto& clip = state.Clip;

        // Update time
        m_CurrentTime += deltaTime * m_Speed * state.Speed;

        // Handle non-looping animations
        if (!clip->IsLooping() && m_CurrentTime >= clip->GetDuration())
        {
            m_CurrentTime = clip->GetDuration();
            // Animation finished - could emit event here in future
        }
    }

    const AnimationFrame* Animator::GetCurrentFrame() const
    {
        if (m_CurrentAnimation.empty())
            return nullptr;

        auto it = m_Animations.find(m_CurrentAnimation);
        if (it == m_Animations.end())
            return nullptr;

        const auto& clip = it->second.Clip;
        return &clip->GetFrameAtTime(m_CurrentTime);
    }

    Ref<SubTexture2D> Animator::GetCurrentTexture() const
    {
        const auto* frame = GetCurrentFrame();
        return frame ? frame->Texture : nullptr;
    }

    bool Animator::HasAnimation(const std::string& name) const
    {
        return m_Animations.find(name) != m_Animations.end();
    }

    std::vector<std::string> Animator::GetAnimationNames() const
    {
        std::vector<std::string> names;
        names.reserve(m_Animations.size());
        
        for (const auto& [name, state] : m_Animations)
        {
            names.push_back(name);
        }
        
        return names;
    }

} // namespace Engine

