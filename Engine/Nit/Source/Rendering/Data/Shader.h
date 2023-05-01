#pragma once
#include "Core/Asset/Asset.h"

namespace Nit
{
    class Shader : public Asset
    {
    public:
        Shader(const std::string& name, const std::string& path, const Id& id, bool bReadFromFile = true);

        bool Load() override;
        bool Unload() override;
        
        bool ReadFromFile(const std::string& fileLocation, std::string& vertexSource, std::string& fragmentSource);
        bool Initialized() const { return m_bInitialized; }

        void Compile(const std::string& vertexSource, const std::string& fragmentSource);
        
        void SetUniformMat4(const char* uniformName, const glm::mat4& mat) const;
        void SetUniformVec4(const char* uniformName, const glm::vec4& vec) const;
        void SetUniformInt(const char* uniformName, int num);
        void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size);
        
        void Bind() const;
        void Unbind() const;
        
    private:
        uint32_t m_ShaderId{0};
        bool m_bInitialized{false};
        bool m_bReadFromFile{true};
        
        RTTR_ENABLE(Asset)
    };
}
