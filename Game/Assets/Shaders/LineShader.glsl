#type vertex
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

#type fragment
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
    //float alpha = smoothstep(0.0, v_Fade, v_LocalPosition.x);
    FragColor = vec4(v_TintColor.rgb, 1);
    EntityID = v_EntityID;
}