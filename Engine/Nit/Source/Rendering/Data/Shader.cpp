#include "Shader.h"
#include <glad/glad.h>

namespace Nit
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Shader>("Shader")
            .constructor<const std::string&, const std::string&, Id&>();
    }
    
    Shader::Shader(const std::string& name, const std::string& path, const Id& id)
        : Asset(name, path, id)
    {
    }

    bool Shader::Load()
    {
        const std::string absolutePath = GetAbsolutePath();
        if (absolutePath == "None")
            return true;
        
        return ReadFromFile(absolutePath, m_VertexSource, m_FragmentSource);
    }

    void Shader::Initialize()
    {
        Compile();
    }

    bool Shader::Unload()
    {
        glDeleteProgram(m_ShaderId);
        return true;
    }

    void Shader::Compile()
    {
        Compile(m_VertexSource, m_FragmentSource);
    }

    void Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource)
    {
        if (m_bCompiled) return;
        
        // Create an empty vertex shader handle
        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
            printf("Error compiling the vertex shader: '%s'\n", infoLog.data());
            
            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            assert(false && "Vertex shader compilation failed!");
        }

        // Create an empty fragment shader handle
        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, nullptr);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
            printf("Error compiling the fragment shader: '%s'\n", infoLog.data());
            
            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            assert(false && "Fragment shader compilation failed!");
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_ShaderId = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(m_ShaderId, vertexShader);
        glAttachShader(m_ShaderId, fragmentShader);

        // Link our program
        glLinkProgram(m_ShaderId);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_ShaderId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);
            
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ShaderId, maxLength, &maxLength, infoLog.data());

            // We don't need the program anymore.
            glDeleteProgram(m_ShaderId);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            
            assert(false && "Shader linkage failed!");
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_ShaderId, vertexShader);
        glDetachShader(m_ShaderId, fragmentShader);

        m_bCompiled = true;
    }

    bool Shader::ReadFromFile(const std::string& fileLocation, std::string& vertexSource, std::string& fragmentSource)
    {
        static std::string content;
        std::ifstream fileStream(fileLocation, std::ios::in);

        if (!fileStream.is_open())
        {
            printf("Failed to read %s! File doesn't exist.\n", fileLocation.c_str());
            return false;
        }
        
        std::string line;

        enum class ShaderType { None, Vertex, Fragment };
        ShaderType currentShader = ShaderType::None;
        
        while (!fileStream.eof())
        {
            std::getline(fileStream, line);

            if (line.find("#type") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    currentShader = ShaderType::Vertex;
                    continue;
                }

                if (line.find("fragment") != std::string::npos)
                {
                    currentShader = ShaderType::Fragment;
                    continue;
                }
            }

            switch (currentShader)
            {
            case ShaderType::None: continue;
            case ShaderType::Vertex: vertexSource.append(line + "\n"); continue;
            case ShaderType::Fragment: fragmentSource.append(line + "\n"); continue;
            }
        }

        fileStream.close();
        return true;
    }

    void Shader::SetUniformMat4(const char* uniformName, const Mat4& mat) const
    {
        const uint32_t id = glGetUniformLocation(m_ShaderId, uniformName);
        glUniformMatrix4fv(id, 1, false, glm::value_ptr(mat));
    }

    void Shader::SetUniformVec4(const char* uniformName, const Vec4& vec) const
    {
        const uint32_t id = glGetUniformLocation(m_ShaderId, uniformName);
        glUniform4fv(id, 1, glm::value_ptr(vec));
    }

    void Shader::SetUniformInt(const char* uniformName, const int num)
    {
        const uint32_t id = glGetUniformLocation(m_ShaderId, uniformName);
        glUniform1i(id, num);
    }

    void Shader::SetUniformIntArray(const char* uniformName, const int32_t* array, const int32_t size)
    {
        const uint32_t id = glGetUniformLocation(m_ShaderId, uniformName);
        glUniform1iv(id, size, array);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ShaderId);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }
}