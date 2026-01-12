#include "Engine/Audio/AudioEngine.h"
#include "Engine/Core/Logger.h"
#include <glm/glm.hpp>

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

namespace Engine {

    static ALCdevice* s_AudioDevice = nullptr;
    static ALCcontext* s_AudioContext = nullptr;
    static float s_MasterVolume = 1.0f;

    void AudioEngine::Init() {
        // Open default audio device
        s_AudioDevice = alcOpenDevice(nullptr);
        if (!s_AudioDevice) {
            GE_CORE_ERROR("Failed to open audio device!");
            return;
        }
        
        // Create audio context
        s_AudioContext = alcCreateContext(s_AudioDevice, nullptr);
        if (!s_AudioContext) {
            GE_CORE_ERROR("Failed to create audio context!");
            alcCloseDevice(s_AudioDevice);
            s_AudioDevice = nullptr;
            return;
        }
        
        // Make context current
        alcMakeContextCurrent(s_AudioContext);
        
        // Set listener defaults
        ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
        ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f,  // "at" vector
                                  0.0f, 1.0f, 0.0f }; // "up" vector
        
        alListenerfv(AL_POSITION, listenerPos);
        alListenerfv(AL_VELOCITY, listenerVel);
        alListenerfv(AL_ORIENTATION, listenerOri);
        alListenerf(AL_GAIN, s_MasterVolume);
        
        GE_CORE_INFO("Audio Engine initialized");
        GE_CORE_INFO("  Device: {}", alcGetString(s_AudioDevice, ALC_DEVICE_SPECIFIER));
        GE_CORE_INFO("  Vendor: {}", alGetString(AL_VENDOR));
        GE_CORE_INFO("  Version: {}", alGetString(AL_VERSION));
        GE_CORE_INFO("  Renderer: {}", alGetString(AL_RENDERER));
    }

    void AudioEngine::Shutdown() {
        if (s_AudioContext) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(s_AudioContext);
            s_AudioContext = nullptr;
        }
        
        if (s_AudioDevice) {
            alcCloseDevice(s_AudioDevice);
            s_AudioDevice = nullptr;
        }
        
        GE_CORE_INFO("Audio Engine shut down");
    }

    void AudioEngine::SetMasterVolume(float volume) {
        s_MasterVolume = glm::clamp(volume, 0.0f, 1.0f);
        alListenerf(AL_GAIN, s_MasterVolume);
    }

    float AudioEngine::GetMasterVolume() {
        return s_MasterVolume;
    }

    void AudioEngine::SetListenerPosition(float x, float y, float z) {
        ALfloat pos[] = { x, y, z };
        alListenerfv(AL_POSITION, pos);
    }

    void AudioEngine::SetListenerVelocity(float x, float y, float z) {
        ALfloat vel[] = { x, y, z };
        alListenerfv(AL_VELOCITY, vel);
    }

    void AudioEngine::SetListenerOrientation(float atX, float atY, float atZ, 
                                            float upX, float upY, float upZ) {
        ALfloat ori[] = { atX, atY, atZ, upX, upY, upZ };
        alListenerfv(AL_ORIENTATION, ori);
    }

    const char* AudioEngine::GetDeviceName() {
        return s_AudioDevice ? alcGetString(s_AudioDevice, ALC_DEVICE_SPECIFIER) : "None";
    }

    const char* AudioEngine::GetVendor() {
        return alGetString(AL_VENDOR);
    }

    const char* AudioEngine::GetVersion() {
        return alGetString(AL_VERSION);
    }

} // namespace Engine

