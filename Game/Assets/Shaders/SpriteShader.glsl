#type vertex
#version 410 core

layout(location = 0)  vec3 a_Position;
layout(location = 1)  vec4 a_Color;
layout(location = 2)  vec2 a_UV;
layout(location = 3)  int  a_TextureSlot;
layout(location = 4)  int  a_EntityID;

uniform mat4 u_ProjectionViewMatrix;

struct VertexOutput
{
    vec4 Color;
    vec2 UV;
    int  TextureSlot;
    int  EntityID;
}

// Vertex output
out VertexOutput vo;

void main()
{
    gl_Position    = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
    
    vo.Color       = a_Color;
    vo.UV          = a_UV;
    vo.TextureSlot = a_TextureSlot;
    vo.EntityID    = a_EntityID;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  EntityID;

// Vertex input
in VertexOutput vi;

uniform sampler2D u_TextureSlots[32];

void main()
{
    EntityID = vi.EntityID;
    FragColor = texture(u_TextureSlots[vi.TextureSlot], vi.UV) * vi.Color;       
}