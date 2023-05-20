#pragma once
#include "RendererAPI.h"
#include "Data/VertexArray.h"
#include "Data/Shader.h"

namespace Nit
{
    class RenderCommand
    {
    public:
        RenderCommand();

        virtual const char* GetName() const = 0;
        virtual void Execute() = 0;
        
    protected:
        const Unique<RendererAPI>& GetRendererAPI() const { return m_RendererAPI; }
    
    private:
        Unique<RendererAPI> m_RendererAPI; 
    };
    
    class SetViewPortCommand : public RenderCommand
    {
    public:
        SetViewPortCommand(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
            : RenderCommand(), m_X(x), m_Y(y), m_Width(width), m_Height(height)
        {}

        const char* GetName() const override { return "SetViewport"; }
        
        void Execute() override
        {
            GetRendererAPI()->SetViewport(m_X, m_Y, m_Width, m_Height);
        }

    private:
        uint32_t m_X, m_Y, m_Width, m_Height; 
    };

    class SetClearColorCommand : public RenderCommand
    {
    public:
        SetClearColorCommand(const Vec4& clearColor)
            : RenderCommand()
            , m_ClearColor(clearColor)
        {}

        const char* GetName() const override { return "SetClearColor"; }

        void Execute() override
        {
            GetRendererAPI()->SetClearColor(m_ClearColor);
        }

    private:
        Vec4 m_ClearColor;
    };

    class ClearCommand : public RenderCommand
    {
    public:

        const char* GetName() const override { return "Clear"; }
        
        void Execute() override
        {
            GetRendererAPI()->Clear();
        }
    };

    class SetBlendingModeCommand : public RenderCommand
    {
        const char* GetName() const override { return "Set Blending mode"; }

    public:
        SetBlendingModeCommand(const BlendingMode blendingMode)
            : RenderCommand()
            , m_BlendingMode(blendingMode)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetBlendingMode(m_BlendingMode);
        }

    private:
        BlendingMode m_BlendingMode;
    };
    
    class SetBlendingEnabledCommand : public RenderCommand
    {
        const char* GetName() const override { return "Set Blending enabled"; }

    public:
        SetBlendingEnabledCommand(const bool enabled)
            : RenderCommand()
            , m_Enabled(enabled)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetBlendingEnabled(m_Enabled);
        }

    private:
        bool m_Enabled;
    };

    class SetDepthTestEnabledCommand : public RenderCommand
    {
        const char* GetName() const override { return "Set Blending enabled"; }

    public:
        SetDepthTestEnabledCommand(const bool enabled)
            : RenderCommand()
            , m_Enabled(enabled)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetDepthTestEnabled(m_Enabled);
        }

    private:
        bool m_Enabled;
    };

    class SetUniformCommand : public RenderCommand
    {
    public:
        
        SetUniformCommand(const Shared<Shader>& shader, const char* uniformName)
            : RenderCommand()
            , m_Shader(shader)
            , m_UniformName(uniformName)
        {}
        
        void Execute() override
        {
            m_Shader->Bind();
        }

    protected:
        const Shared<Shader> m_Shader;
        const char* m_UniformName;
    };
    
    class SetUniformMat4Command : public SetUniformCommand
    {
    public:
        
        SetUniformMat4Command(const Shared<Shader>& shader, const char* uniformName, const Mat4& mat)
            : SetUniformCommand(shader, uniformName)
            , m_Mat(mat)
        {}

        const char* GetName() const override { return "SetUniformMat4"; }
        
        void Execute() override
        {
            SetUniformCommand::Execute();
            m_Shader->SetUniformMat4(m_UniformName, m_Mat);
        }

    private:
        const Mat4 m_Mat;
    };
    
    class SetUniformVec4Command : public SetUniformCommand
    {
    public:
        
        SetUniformVec4Command(const Shared<Shader>& shader, const char* uniformName, const Vec4& vec)
            : SetUniformCommand(shader, uniformName)
            , m_Vec4(vec)
        {}

        const char* GetName() const override { return "SetUniformVec4"; }
        
        void Execute() override
        {
            SetUniformCommand::Execute();
            m_Shader->SetUniformVec4(m_UniformName, m_Vec4);
        }

    private:
        const Vec4 m_Vec4;
    };
    
    class SetUniformIntCommand : public SetUniformCommand
    {
    public:
        
        SetUniformIntCommand(const Shared<Shader>& shader, const char* uniformName, const int num)
            : SetUniformCommand(shader, uniformName)
            , m_Num(num)
        {}

        const char* GetName() const override { return "SetUniformInt"; }
        
        void Execute() override
        {
            SetUniformCommand::Execute();
            m_Shader->SetUniformInt(m_UniformName, m_Num);
        }

    private:
        const int m_Num;
    };

    class DrawElementsCommand : public RenderCommand
    {
    public:
        DrawElementsCommand(const Shared<VertexArray>& vertexArray, const uint32_t elementCount)
            : RenderCommand()
            , m_VertexArray(vertexArray)
            , m_ElementCount(elementCount)
        {}

        const char* GetName() const override { return "DrawElements"; }
        
        void Execute() override
        {
            GetRendererAPI()->DrawElements(m_VertexArray, m_ElementCount);
        }

    private:
        Shared<VertexArray> m_VertexArray;
        const uint32_t m_ElementCount;
    };
}
