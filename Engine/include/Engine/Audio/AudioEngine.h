#pragma once

#include "Engine/Core/Base.h"
#include <string>

namespace Engine {

    // Audio engine - manages OpenAL context and audio playback
    class AudioEngine {
    public:
        static void Init();
        static void Shutdown();
        
        // Volume controls (0.0 to 1.0)
        static void SetMasterVolume(float volume);
        static float GetMasterVolume();
        
        // Listener position (for 3D audio)
        static void SetListenerPosition(float x, float y, float z);
        static void SetListenerVelocity(float x, float y, float z);
        static void SetListenerOrientation(float atX, float atY, float atZ, 
                                          float upX, float upY, float upZ);
        
        // Get OpenAL context info
        static const char* GetDeviceName();
        static const char* GetVendor();
        static const char* GetVersion();
        
    private:
        AudioEngine() = default;
    };

} // namespace Engine

