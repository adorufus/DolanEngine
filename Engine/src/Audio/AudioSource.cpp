#include "Engine/Audio/AudioSource.h"
#include "Engine/Core/Logger.h"

#ifdef __APPLE__
    #include <OpenAL/al.h>
#else
    #include <AL/al.h>
#endif

namespace Engine {

    AudioSource::AudioSource() {
        alGenSources(1, &m_SourceID);
        
        // Set defaults
        alSourcef(m_SourceID, AL_PITCH, m_Pitch);
        alSourcef(m_SourceID, AL_GAIN, m_Volume);
        alSourcei(m_SourceID, AL_LOOPING, m_Looping ? AL_TRUE : AL_FALSE);
        
        // Default 3D settings
        alSourcef(m_SourceID, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(m_SourceID, AL_MAX_DISTANCE, 100.0f);
        alSourcef(m_SourceID, AL_ROLLOFF_FACTOR, 1.0f);
    }

    AudioSource::~AudioSource() {
        if (m_SourceID) {
            alDeleteSources(1, &m_SourceID);
        }
    }

    void AudioSource::Play() {
        if (m_Buffer && m_Buffer->GetBufferID()) {
            alSourcei(m_SourceID, AL_BUFFER, m_Buffer->GetBufferID());
            alSourcePlay(m_SourceID);
        }
    }

    void AudioSource::Pause() {
        alSourcePause(m_SourceID);
    }

    void AudioSource::Stop() {
        alSourceStop(m_SourceID);
    }

    bool AudioSource::IsPlaying() const {
        ALint state;
        alGetSourcei(m_SourceID, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    bool AudioSource::IsPaused() const {
        ALint state;
        alGetSourcei(m_SourceID, AL_SOURCE_STATE, &state);
        return state == AL_PAUSED;
    }

    void AudioSource::SetBuffer(const Ref<AudioBuffer>& buffer) {
        m_Buffer = buffer;
    }

    void AudioSource::SetPosition(const glm::vec3& position) {
        alSource3f(m_SourceID, AL_POSITION, position.x, position.y, position.z);
    }

    void AudioSource::SetVelocity(const glm::vec3& velocity) {
        alSource3f(m_SourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    void AudioSource::SetDirection(const glm::vec3& direction) {
        alSource3f(m_SourceID, AL_DIRECTION, direction.x, direction.y, direction.z);
    }

    void AudioSource::SetVolume(float volume) {
        m_Volume = glm::clamp(volume, 0.0f, 1.0f);
        alSourcef(m_SourceID, AL_GAIN, m_Volume);
    }

    void AudioSource::SetPitch(float pitch) {
        m_Pitch = glm::clamp(pitch, 0.5f, 2.0f);
        alSourcef(m_SourceID, AL_PITCH, m_Pitch);
    }

    void AudioSource::SetLooping(bool loop) {
        m_Looping = loop;
        alSourcei(m_SourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

    void AudioSource::SetReferenceDistance(float distance) {
        alSourcef(m_SourceID, AL_REFERENCE_DISTANCE, distance);
    }

    void AudioSource::SetMaxDistance(float distance) {
        alSourcef(m_SourceID, AL_MAX_DISTANCE, distance);
    }

    void AudioSource::SetRolloffFactor(float rolloff) {
        alSourcef(m_SourceID, AL_ROLLOFF_FACTOR, rolloff);
    }

    void AudioSource::Set2D(bool is2D) {
        if (is2D) {
            // For 2D audio, position at listener and disable spatialization
            alSource3f(m_SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
            alSourcei(m_SourceID, AL_SOURCE_RELATIVE, AL_TRUE);
        } else {
            alSourcei(m_SourceID, AL_SOURCE_RELATIVE, AL_FALSE);
        }
    }

    Ref<AudioSource> AudioSource::Create() {
        return CreateRef<AudioSource>();
    }

} // namespace Engine

