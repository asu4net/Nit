#pragma once
#include "RenderAPI.h"
#include "NitRender/Objects/VertexArray.h"
#include "NitRender/Objects/Shader.h"

namespace Nit
{
    class CRenderCommand;

    class CRenderCommandQueue
    {
    public:
        static bool IsEmpty() { return m_CommandQueue.empty(); }

        template<typename T, typename ...TArgs>
        static void Submit(TArgs&& ...args)
        {
            TSharedPtr<T> command = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_CommandQueue.push_back(command);
        }

        static void ExecuteNext();

    private:
        inline static TDynamicArray<TSharedPtr<CRenderCommand>> m_CommandQueue;
    };

    class CRenderCommand
    {
    public:
        CRenderCommand(const TSharedPtr<CRenderAPI>& api);

        virtual const char* GetName() const = 0;
        virtual void Execute() = 0;
        
    protected:
        const TSharedPtr<CRenderAPI>& GetRendererAPI() const { return m_RendererAPI; }
    
    private:
        TSharedPtr<CRenderAPI> m_RendererAPI;
    };
    
    class SetViewPortCommand : public CRenderCommand
    {
    public:
        SetViewPortCommand(const TSharedPtr<CRenderAPI>& api, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
            : CRenderCommand(api), m_X(x), m_Y(y), m_Width(width), m_Height(height)
        {}

        const char* GetName() const override { return "SetViewport"; }
        
        void Execute() override
        {
            GetRendererAPI()->SetViewport(m_X, m_Y, m_Width, m_Height);
        }

    private:
        uint32_t m_X, m_Y, m_Width, m_Height; 
    };

    class SetClearColorCommand : public CRenderCommand
    {
    public:
        SetClearColorCommand(const TSharedPtr<CRenderAPI>& api, const CColor& clearColor)
            : CRenderCommand(api)
            , m_ClearColor(clearColor)
        {}

        const char* GetName() const override { return "SetClearColor"; }

        void Execute() override
        {
            GetRendererAPI()->SetClearColor(m_ClearColor);
        }

    private:
        CColor m_ClearColor;
    };

    class ClearCommand : public CRenderCommand
    {
    public:
        ClearCommand(const TSharedPtr<CRenderAPI>& api)
            : CRenderCommand(api)
        {}

        const char* GetName() const override { return "Clear"; }
        
        void Execute() override
        {
            GetRendererAPI()->Clear();
        }
    };

    class SetBlendingModeCommand : public CRenderCommand
    {
        const char* GetName() const override { return "Set Blending mode"; }

    public:
        SetBlendingModeCommand(const TSharedPtr<CRenderAPI>& api, const EBlendingMode blendingMode)
            : CRenderCommand(api)
            , m_BlendingMode(blendingMode)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetBlendingMode(m_BlendingMode);
        }

    private:
        EBlendingMode m_BlendingMode;
    };
    
    class SetBlendingEnabledCommand : public CRenderCommand
    {
        const char* GetName() const override { return "Set Blending enabled"; }

    public:
        SetBlendingEnabledCommand(const TSharedPtr<CRenderAPI>& api, const bool enabled)
            : CRenderCommand(api)
            , m_Enabled(enabled)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetBlendingEnabled(m_Enabled);
        }

    private:
        bool m_Enabled;
    };

    class SetDepthTestEnabledCommand : public CRenderCommand
    {
        const char* GetName() const override { return "Set Blending enabled"; }

    public:
        SetDepthTestEnabledCommand(const TSharedPtr<CRenderAPI>& api, const bool enabled)
            : CRenderCommand(api)
            , m_Enabled(enabled)
        {}
        
        void Execute() override
        {
            GetRendererAPI()->SetDepthTestEnabled(m_Enabled);
        }

    private:
        bool m_Enabled;
    };

    class SetUniformCommand : public CRenderCommand
    {
    public:
        
        SetUniformCommand(const TSharedPtr<CRenderAPI>& api, const TSharedPtr<CShader>& shader, const char* uniformName)
            : CRenderCommand(api)
            , m_Shader(shader)
            , m_UniformName(uniformName)
        {}
        
        void Execute() override
        {
            m_Shader->Bind();
        }

    protected:
        const TSharedPtr<CShader> m_Shader;
        const char* m_UniformName;
    };
    
    class SetUniformMat4Command : public SetUniformCommand
    {
    public:
        
        SetUniformMat4Command(const TSharedPtr<CRenderAPI>& api, const TSharedPtr<CShader>& shader, const char* uniformName, const CMatrix4& mat)
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
        const CMatrix4 m_Mat;
    };
    
    class SetUniformVec4Command : public SetUniformCommand
    {
    public:
        
        SetUniformVec4Command(const TSharedPtr<CRenderAPI>& api, const TSharedPtr<CShader>& shader, const char* uniformName, const CVector4& vec)
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
        const CVector4 m_Vec4;
    };
    
    class SetUniformIntCommand : public SetUniformCommand
    {
    public:
        
        SetUniformIntCommand(const TSharedPtr<CRenderAPI>& api, const TSharedPtr<CShader>& shader, const char* uniformName, const int num)
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

    class DrawElementsCommand : public CRenderCommand
    {
    public:
        DrawElementsCommand(const TSharedPtr<CRenderAPI>& api, const TSharedPtr<CVertexArray>& vertexArray, const uint32_t elementCount)
            : CRenderCommand(api)
            , m_VertexArray(vertexArray)
            , m_ElementCount(elementCount)
        {}

        const char* GetName() const override { return "DrawElements"; }
        
        void Execute() override
        {
            GetRendererAPI()->DrawElements(m_VertexArray, m_ElementCount);
        }

    private:
        TSharedPtr<CVertexArray> m_VertexArray;
        const uint32_t m_ElementCount;
    };
}
