#type vertex
#version 410 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_TintColor;
layout(location = 2) in vec3  a_LocalPosition;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int   a_EntityID;

// Vertex output
out vec4     v_TintColor;
out vec3     v_LocalPosition;
out float    v_Thickness;
out float    v_Fade;
flat out int v_EntityID;

void main()
{
    gl_Position     = vec4(a_Position, 1.0);
    
    v_TintColor     = a_TintColor;
    v_LocalPosition = a_LocalPosition;
    v_Thickness     = a_Thickness;
    v_Fade          = a_Fade;
    v_EntityID      = a_EntityID;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  EntityID;

// Vertex input
in vec4     v_TintColor;
in vec3     v_LocalPosition;
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