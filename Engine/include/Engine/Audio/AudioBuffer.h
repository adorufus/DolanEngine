#pragma once

#include "Engine/Core/Base.h"
#include <string>

namespace Engine {

    // Audio buffer - holds audio data
    class AudioBuffer {
    public:
        AudioBuffer(const std::string& filepath);
        ~AudioBuffer();
        
        uint32_t GetBufferID() const { return m_BufferID; }
        float GetDuration() const { return m_Duration; }
        const std::string& GetFilepath() const { return m_Filepath; }
        
        // Factory method
        static Ref<AudioBuffer> Create(const std::string& filepath);
        
    private:
        bool LoadWAV(const std::string& filepath);
        
    private:
        uint32_t m_BufferID = 0;
        float m_Duration = 0.0f;
        std::string m_Filepath;
    };

} // namespace Engine

