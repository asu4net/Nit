#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in int a_TextureSlot;

uniform mat4 u_ProjectionViewMatrix;

// Vertex output
out vec4 v_Color;
out vec2 v_UV;
flat out int v_TextureSlot;

void main()
{
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
    
    v_Color = a_Color;
    v_UV = a_UV;
    v_TextureSlot = a_TextureSlot;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 color;

// Vertex input
in vec4 v_Color;
flat in int v_TextureSlot;
in vec2 v_UV;

uniform sampler2D u_TextureSlots[32];

void main()
{
    color = texture(u_TextureSlots[v_TextureSlot], v_UV) * v_Color;
}