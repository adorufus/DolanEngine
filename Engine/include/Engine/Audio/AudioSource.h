#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Audio/AudioBuffer.h"
#include <glm/glm.hpp>

namespace Engine {

    // Audio source - plays audio buffers in 3D space
    class AudioSource {
    public:
        AudioSource();
        ~AudioSource();

        // Playback control
        void Play();
        void Pause();
        void Stop();
        
        bool IsPlaying() const;
        bool IsPaused() const;
        
        // Buffer
        void SetBuffer(const Ref<AudioBuffer>& buffer);
        Ref<AudioBuffer> GetBuffer() const { return m_Buffer; }
        
        // Properties
        void SetPosition(const glm::vec3& position);
        void SetVelocity(const glm::vec3& velocity);
        void SetDirection(const glm::vec3& direction);
        
        void SetVolume(float volume);          // 0.0 to 1.0
        void SetPitch(float pitch);            // 0.5 to 2.0 typically
        void SetLooping(bool loop);
        
        // 3D audio properties
        void SetReferenceDistance(float distance);  // Distance at which volume = 1.0
        void SetMaxDistance(float distance);        // Distance at which volume = 0.0
        void SetRolloffFactor(float rolloff);       // How quickly volume decreases
        
        // 2D audio (disables 3D positioning)
        void Set2D(bool is2D);
        
        float GetVolume() const { return m_Volume; }
        float GetPitch() const { return m_Pitch; }
        bool IsLooping() const { return m_Looping; }
        
        // Factory method
        static Ref<AudioSource> Create();
        
    private:
        uint32_t m_SourceID = 0;
        Ref<AudioBuffer> m_Buffer;
        
        float m_Volume = 1.0f;
        float m_Pitch = 1.0f;
        bool m_Looping = false;
    };

} // namespace Engine

