#include "NitPCH.h"
#include "AudioClip.h"
#include <alc/alcmain.h>

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<AudioClip>("AudioClip")
       .constructor<>();
}

namespace Nit
{
    bool AudioClip::Load()
    {
        const String absolutePath = GetAssetData().AbsolutePath;

        if (absolutePath.empty())
            return false;
        
        char buffer[4];
        std::ifstream in(absolutePath, std::ios::binary);

        if (!in)
            return false;
        
        // ChunkID (“RIFF”)
        uint32_t chunkId{0};
        in.read(buffer, 4);
        if (strncmp(buffer, "RIFF", 4) != 0)
            assert(false && "This is not a valid WAV file");

        memcpy(&chunkId, buffer, 4);

        // RiffChunkSize
        in.read(buffer, 4);
    
        // Format "WAVE"
        in.read(buffer, 4);
        if (strncmp(buffer, "WAVE", 4) != 0)
            assert(false && "This is not a valid WAV file");
    
        // SubChunkId "fmt"
        in.read(buffer, 4);
        if (strncmp(buffer, "fmt", 3) != 0)
            assert(false && "This is not a valid WAV file");

        // FmtChunkSize
        uint32_t chunkSize{0};
        in.read(buffer, 4);
        memcpy(&chunkSize, buffer, 4);

        const bool readExtraParams = chunkSize > 16;

        // AudioFormat
        in.read(buffer, 2);
    
        // Channels
        in.read(buffer, 2);
        uint32_t channel{0};
        memcpy(&channel, buffer, 4);
        const bool isStereo = channel != 1;

        // SampleRate
        in.read(buffer, 4);
        memcpy(&m_Frec, buffer, 4);

        // ByteRate
        in.read(buffer, 4);

        // BlockAlign
        in.read(buffer, 2);
    
        // BitsPerSample
        in.read(buffer, 2);
        uint32_t bitsPerSample{0};
        memcpy(&bitsPerSample, buffer, 2);

        if (bitsPerSample == 8)
            m_Format = isStereo ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
        else if (bitsPerSample == 16)
            m_Format = isStereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

        // ExtraParamsSize
        if (readExtraParams)
        {
            in.read(buffer, 2);
            uint32_t extraParamSize{0};
            memcpy(&extraParamSize, buffer, 2);
            // ExtraParams
            in.read(buffer, extraParamSize);
        }

        // Data
        while(strncmp(buffer, "data", 4) != 0)
            in.read(buffer, 4);

        // DataSize
        in.read(buffer, 4);
        memcpy(&m_Size, buffer, 4);

        m_Data = new char[m_Size];
        in.read(m_Data, m_Size);

        alGenBuffers(1, &m_BufferId);
        if (alGetError() != AL_NO_ERROR)
            assert(false && "AL error.");

        alBufferData(m_BufferId, m_Format, m_Data, m_Size, m_Frec);
        
        return true;
    }

    void AudioClip::Unload()
    {
        alDeleteBuffers(1, &m_BufferId);
        delete[] m_Data;
        m_Data = nullptr;
    }
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(AudioClip)
}
