#include "RawShaderStrings.h"

namespace Nit
{
    constexpr const char* SpriteVertShaderStr = R"(
        #version 410 core

        layout(location = 0) in vec3  a_Position;
        layout(location = 1) in vec3  a_LocalPosition;
        layout(location = 2) in vec4  a_TintColor;
        layout(location = 3) in vec2  a_UVCoords;
        layout(location = 4) in int   a_TextureSlot;
        layout(location = 5) in float a_Time;
        layout(location = 6) in int   a_EntityID;
        
        // Vertex output
        out vec4      v_TintColor;
        out vec2      v_UVCoords;
        flat out int  v_TextureSlot;
        flat out int  v_EntityID;
        
        void main()
        {
            gl_Position   = vec4(a_Position, 1.0);
            
            v_TintColor   = a_TintColor;
            v_UVCoords    = a_UVCoords;
            v_TextureSlot = a_TextureSlot;
            v_EntityID    = a_EntityID;
        }
    )";

    constexpr const char* SpriteFragShaderStr = R"(
        #version 410 core

        layout(location = 0) out vec4 FragColor;
        layout(location = 1) out int  EntityID;
        
        // Vertex input
        in vec4      v_TintColor;
        in vec2      v_UVCoords;
        flat in int  v_TextureSlot;
        flat in int  v_EntityID;
        
        uniform sampler2D u_TextureSlots[32];
        
        void main()
        {
            EntityID = v_EntityID;
            FragColor = texture(u_TextureSlots[v_TextureSlot], v_UVCoords) * v_TintColor;       
        }
    )";
    
    constexpr const char* CircleVertShaderStr = R"(
        #version 410 core
        
        layout(location = 0) in vec3  a_Position;
        layout(location = 1) in vec3  a_LocalPosition;
        layout(location = 2) in vec4  a_TintColor;
        layout(location = 3) in float a_Thickness;
        layout(location = 4) in float a_Fade;
        layout(location = 5) in float a_Time;
        layout(location = 6) in int   a_EntityID;
        
        // Vertex output
        out vec3     v_LocalPosition;
        out vec4     v_TintColor;
        out float    v_Thickness;
        out float    v_Fade;
        flat out int v_EntityID;
        
        void main()
        {
            gl_Position     = vec4(a_Position, 1.0);
            
            v_LocalPosition = a_LocalPosition;
            v_TintColor     = a_TintColor;
            v_Thickness     = a_Thickness;
            v_Fade          = a_Fade;
            v_EntityID      = a_EntityID;
        }
    )";

    constexpr const char* CircleFragShaderStr = R"(
        #version 410 core

        layout(location = 0) out vec4 FragColor;
        layout(location = 1) out int  EntityID;
        
        // Vertex input
        in vec3     v_LocalPosition;
        in vec4     v_TintColor;
        in float    v_Thickness;
        in float    v_Fade;
        flat in int v_EntityID;
        
        void main()
        {
            vec2 localPos = vec2(v_LocalPosition.x * 2, v_LocalPosition.y * 2);
            float d = 1.0 - length(localPos);
            float alpha = smoothstep(0.0, v_Fade, d);
        
            alpha *= smoothstep(v_Thickness + v_Fade, v_Thickness, d);
        
            FragColor = vec4(v_TintColor.rgb, alpha);
            EntityID = v_EntityID;    
        }
    )";

    constexpr const char* LineVertShaderStr = R"(
        #version 410 core

        layout(location = 0) in vec3  a_Position;
        layout(location = 1) in vec3  a_LocalPosition;
        layout(location = 2) in vec4  a_TintColor;
        layout(location = 3) in float a_Fade;
        layout(location = 4) in float a_Time;
        layout(location = 5) in int   a_EntityID;
        
        // Vertex output
        out vec3     v_LocalPosition;
        out vec4     v_TintColor;
        out float    v_Fade;
        flat out int v_EntityID;
        
        void main()
        {
            gl_Position     = vec4(a_Position, 1.0);
            
            v_LocalPosition = a_LocalPosition;
            v_TintColor     = a_TintColor;
            v_Fade          = a_Fade;
            v_EntityID      = a_EntityID;
        }
    )";
    
    constexpr const char* LineFragShaderStr = R"(
        #version 410 core

        layout(location = 0) out vec4 FragColor;
        layout(location = 1) out int  EntityID;
        
        // Vertex input
        in vec3     v_LocalPosition;
        in vec4     v_TintColor;
        in float    v_Fade;
        flat in int v_EntityID;
        
        void main()
        {
            FragColor = v_TintColor;
            EntityID = v_EntityID;    
        }
    )";

    constexpr const char* ErrorVertShaderStr = R"(
        #version 410 core
        
        void main()
        {
        }
    )";
    
    constexpr const char* ErrorFragShaderStr = R"(
        #version 410 core
        
        layout(location = 0) out vec4 color;
                
        void main()
        {
            color = vec4(0.827, 0.012, 1.0, 1.0);
        }
    )";


    const char* GetSpriteVertShaderStr()
    {
        return SpriteVertShaderStr;
    }

    const char* GetSpriteFragShaderStr()
    {
        return SpriteFragShaderStr;
    }

    const char* GetCircleVertShaderStr()
    {
        return CircleVertShaderStr;
    }

    const char* GetCircleFragShaderStr()
    {
        return CircleFragShaderStr;
    }

    const char* GetLineVertShaderStr()
    {
        return LineVertShaderStr;
    }

    const char* GetLineFragShaderStr()
    {
        return LineFragShaderStr;
    }

    const char* GetErrorVertShaderStr()
    {
        return ErrorVertShaderStr;
    }

    const char* GetErrorFragShaderStr()
    {
        return ErrorFragShaderStr;
    }
}