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
out float     v_Time;
flat out int  v_EntityID;

void main()
{
    gl_Position   = vec4(a_Position, 1.0);
    
    v_TintColor   = a_TintColor;
    v_UVCoords    = a_UVCoords;
    v_TextureSlot = a_TextureSlot;
    v_Time        = a_Time;
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
in float     v_Time;
flat in int  v_EntityID;

uniform sampler2D u_TextureSlots[32];

void main()
{
    vec2 uv = vec2(v_UVCoords);

    // Time varying pixel color
    vec3 color = vec3(
        .25 * sin((uv.x + (v_Time / 5.)) * 10. / 3.14) + .75,
        .25 * cos((uv.y + (v_Time / 7.)) * 10. / 3.14) + .75,
        .25 * sin((uv.x + uv.y + (v_Time / 11.)) * 10. / 3.14) + .75
    );
    
    EntityID = v_EntityID;
    FragColor = vec4(color, 1);       
}