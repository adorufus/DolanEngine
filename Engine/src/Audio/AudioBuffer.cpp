#include "Engine/Audio/AudioBuffer.h"
#include "Engine/Core/Logger.h"

#ifdef __APPLE__
    #include <OpenAL/al.h>
#else
    #include <AL/al.h>
#endif

#include <fstream>
#include <vector>

namespace Engine {

    // Simple WAV file loader
    struct WAVHeader {
        char riff[4];          // "RIFF"
        uint32_t fileSize;
        char wave[4];          // "WAVE"
        char fmt[4];           // "fmt "
        uint32_t fmtSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
    };

    struct WAVData {
        char data[4];          // "data"
        uint32_t dataSize;
    };

    AudioBuffer::AudioBuffer(const std::string& filepath)
        : m_Filepath(filepath)
    {
        if (!LoadWAV(filepath)) {
            GE_CORE_ERROR("Failed to load audio file: {}", filepath);
        }
    }

    AudioBuffer::~AudioBuffer() {
        if (m_BufferID) {
            alDeleteBuffers(1, &m_BufferID);
        }
    }

    bool AudioBuffer::LoadWAV(const std::string& filepath) {
        // Open file
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            GE_CORE_ERROR("Could not open audio file: {}", filepath);
            return false;
        }

        // Read WAV header
        WAVHeader header;
        file.read((char*)&header, sizeof(WAVHeader));
        
        // Verify it's a WAV file
        if (std::string(header.riff, 4) != "RIFF" || 
            std::string(header.wave, 4) != "WAVE") {
            GE_CORE_ERROR("Invalid WAV file: {}", filepath);
            return false;
        }

        // Read data chunk header
        WAVData dataHeader;
        file.read((char*)&dataHeader, sizeof(WAVData));
        
        // Read audio data
        std::vector<char> audioData(dataHeader.dataSize);
        file.read(audioData.data(), dataHeader.dataSize);
        file.close();

        // Determine OpenAL format
        ALenum format = 0;
        if (header.numChannels == 1) {
            format = (header.bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
        } else if (header.numChannels == 2) {
            format = (header.bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
        } else {
            GE_CORE_ERROR("Unsupported number of channels: {}", header.numChannels);
            return false;
        }

        // Create OpenAL buffer
        alGenBuffers(1, &m_BufferID);
        alBufferData(m_BufferID, format, audioData.data(), dataHeader.dataSize, header.sampleRate);

        // Check for errors
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            GE_CORE_ERROR("OpenAL error while loading audio: {}", error);
            return false;
        }

        // Calculate duration
        m_Duration = (float)dataHeader.dataSize / 
                     (header.sampleRate * header.numChannels * (header.bitsPerSample / 8));

        GE_CORE_TRACE("Loaded audio: {} ({:.2f}s, {}Hz, {} channels)", 
                     filepath, m_Duration, header.sampleRate, header.numChannels);
        
        return true;
    }

    Ref<AudioBuffer> AudioBuffer::Create(const std::string& filepath) {
        return CreateRef<AudioBuffer>(filepath);
    }

} // namespace Engine

