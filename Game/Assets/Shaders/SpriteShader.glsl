#type vertex
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

#type fragment
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