#pragma once

namespace Nit
{
    class Shader
    {
    public:
        static std::shared_ptr<Shader> Create(const std::string& fileLocation);
        static std::shared_ptr<Shader> Create();
        
        Shader(const std::string& fileLocation);
        Shader();
        ~Shader();

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
        uint32_t m_ShaderId;
        bool m_bInitialized;
    };
}
