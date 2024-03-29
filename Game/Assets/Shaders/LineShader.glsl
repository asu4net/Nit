#type vertex
#version 410 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec3  a_LocalPosition;
layout(location = 2) in vec4  a_TintColor;
layout(location = 3) in vec2  a_Size;
layout(location = 4) in float a_Fade;
layout(location = 5) in float a_Time;
layout(location = 6) in int   a_EntityID;

// Vertex output
out vec3     v_LocalPosition;
out vec4     v_TintColor;
out vec2     v_Size;
out float    v_Fade;
flat out int v_EntityID;

void main()
{
    gl_Position     = vec4(a_Position, 1.0);
    
    v_LocalPosition = a_LocalPosition;
    v_TintColor     = a_TintColor;
    v_Size          = a_Size;
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
in vec2     v_Size;
in float    v_Fade;
flat in int v_EntityID;

float calculateBoxSDF(vec2 point, vec2 boxRect)
{
   vec2 delta = abs(point) - boxRect;
   return length(max(delta, 0.0)) + min(max(delta.x,delta.y),0.0); 
}

void main()
{
    //float d = calculateBoxSDF(v_LocalPosition.xy, v_Size);
    //float alpha = smoothstep(v_Fade, 0.0, d);

    FragColor = vec4(v_TintColor.rgb, 1);
    EntityID = v_EntityID;
}