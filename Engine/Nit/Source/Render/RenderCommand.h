#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"
#include "RendererShader.h"

namespace Nit
{
    class RenderCommand;

    class RenderCommandQueue
    {
    public:
        static bool IsEmpty() { return m_CommandQueue.empty(); }

        template<typename T, typename ...TArgs>
        static void Submit(TArgs&& ...args)
        {
            SharedPtr<T> command = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_CommandQueue.push_back(command);
        }

        static void ExecuteNext();

    private:
        inline static DynamicArray<SharedPtr<RenderCommand>> m_CommandQueue;
    };

    class RenderCommand
    {
    public:
        RenderCommand(const SharedPtr<RendererAPI>& api);

        virtual const char* GetName() const = 0;
        virtual void Execute() = 0;
        
    protected:
        const SharedPtr<RendererAPI>& GetRendererAPI() const { return m_RendererAPI; }
    
    private:
        SharedPtr<RendererAPI> m_RendererAPI;
    };
    
    class SetViewPortCommand : public RenderCommand
    {
    public:
        SetViewPortCommand(const SharedPtr<RendererAPI>& api, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
            : RenderCommand(api), m_X(x), m_Y(y), m_Width(width), m_Height(height)
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
        SetClearColorCommand(const SharedPtr<RendererAPI>& api, const Color& clearColor)
            : RenderCommand(api)
            , m_ClearColor(clearColor)
        {}

        const char* GetName() const override { return "SetClearColor"; }

        void Execute() override
        {
            GetRendererAPI()->SetClearColor(m_ClearColor);
        }

    private:
        Color m_ClearColor;
    };

    class ClearCommand : public RenderCommand
    {
    public:
        ClearCommand(const SharedPtr<RendererAPI>& api)
            : RenderCommand(api)
        {}

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
        SetBlendingModeCommand(const SharedPtr<RendererAPI>& api, const BlendingMode blendingMode)
            : RenderCommand(api)
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
        SetBlendingEnabledCommand(const SharedPtr<RendererAPI>& api, const bool enabled)
            : RenderCommand(api)
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
        SetDepthTestEnabledCommand(const SharedPtr<RendererAPI>& api, const bool enabled)
            : RenderCommand(api)
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
        
        SetUniformCommand(const SharedPtr<RendererAPI>& api, const SharedPtr<RendererShader>& shader, const char* uniformName)
            : RenderCommand(api)
            , m_Shader(shader)
            , m_UniformName(uniformName)
        {}
        
        void Execute() override
        {
            m_Shader->Bind();
        }

    protected:
        const SharedPtr<RendererShader> m_Shader;
        const char* m_UniformName;
    };
    
    class SetUniformMat4Command : public SetUniformCommand
    {
    public:
        
        SetUniformMat4Command(const SharedPtr<RendererAPI>& api, const SharedPtr<RendererShader>& shader, const char* uniformName, const Matrix4& mat)
            : SetUniformCommand(api, shader, uniformName)
            , m_Mat(mat)
        {}

        const char* GetName() const override { return "SetUniformMat4"; }
        
        void Execute() override
        {
            SetUniformCommand::Execute();
            m_Shader->SetUniformMat4(m_UniformName, m_Mat);
        }

    private:
        const Matrix4 m_Mat;
    };
    
    class SetUniformVec4Command : public SetUniformCommand
    {
    public:
        
        SetUniformVec4Command(const SharedPtr<RendererAPI>& api, const SharedPtr<RendererShader>& shader, const char* uniformName, const Vector4& vec)
            : SetUniformCommand(api, shader, uniformName)
            , m_Vec4(vec)
        {}

        const char* GetName() const override { return "SetUniformVec4"; }
        
        void Execute() override
        {
            SetUniformCommand::Execute();
            m_Shader->SetUniformVec4(m_UniformName, m_Vec4);
        }

    private:
        const Vector4 m_Vec4;
    };
    
    class SetUniformIntCommand : public SetUniformCommand
    {
    public:
        
        SetUniformIntCommand(const SharedPtr<RendererAPI>& api, const SharedPtr<RendererShader>& shader, const char* uniformName, const int num)
            : SetUniformCommand(api, shader, uniformName)
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
        DrawElementsCommand(const SharedPtr<RendererAPI>& api, const SharedPtr<VertexArray>& vertexArray, const uint32_t elementCount)
            : RenderCommand(api)
            , m_VertexArray(vertexArray)
            , m_ElementCount(elementCount)
        {}

        const char* GetName() const override { return "DrawElements"; }
        
        void Execute() override
        {
            GetRendererAPI()->DrawElements(m_VertexArray, m_ElementCount);
        }

    private:
        SharedPtr<VertexArray> m_VertexArray;
        const uint32_t m_ElementCount;
    };
}
