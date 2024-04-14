#include "Shader.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"

namespace Nit
{
    bool CShader::Load()
    {
        const TString absolutePath = GetAssetData().AbsolutePath;
        if (absolutePath == "None")
            return true;

        if (!ReadFromFile(absolutePath, m_VertexSource, m_FragmentSource))
            return false;

        m_RendererShaderId = Renderer::CreateShader(m_VertexSource.c_str(), m_FragmentSource.c_str());
        return true;
    }

    void CShader::Unload()
    {
        Renderer::DestroyShader(m_RendererShaderId);
    }

    bool CShader::ReadFromFile(const TString& fileLocation, TString& vertexSource, TString& fragmentSource)
    {
        static TString content;
        std::ifstream fileStream(fileLocation, std::ios::in);

        if (!fileStream.is_open())
        {
            printf("Failed to read %s! File doesn't exist.\n", fileLocation.c_str());
            return false;
        }

        TString line;

        enum class ShaderType { None, Vertex, Fragment };
        ShaderType currentShader = ShaderType::None;

        while (!fileStream.eof())
        {
            std::getline(fileStream, line);

            if (line.find("#type") != TString::npos)
            {
                if (line.find("vertex") != TString::npos)
                {
                    currentShader = ShaderType::Vertex;
                    continue;
                }

                if (line.find("fragment") != TString::npos)
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
}