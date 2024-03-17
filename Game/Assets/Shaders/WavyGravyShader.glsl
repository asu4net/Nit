#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_TintColor;
layout(location = 3) in vec2 a_UVCoords;
layout(location = 4) in int  a_TextureSlot;
layout(location = 5) in int  a_EntityID;

// Vertex output
out vec3      v_LocalPosition;
out vec4      v_TintColor;
out vec2      v_UVCoords;
flat out int  v_TextureSlot;
flat out int  v_EntityID;

void main()
{
    gl_Position   = vec4(a_Position, 1.0);
    
    v_LocalPosition = a_LocalPosition;
    v_TintColor     = a_TintColor;
    v_UVCoords      = a_UVCoords;
    v_TextureSlot   = a_TextureSlot;
    v_EntityID      = a_EntityID;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  EntityID;

// Vertex input
in vec3      v_LocalPosition;
in vec4      v_TintColor;
in vec2      v_UVCoords;
flat in int  v_TextureSlot;
flat in int  v_EntityID;


uniform sampler2D u_TextureSlots[32];

void main()
{
    // change by uniform
    float u_Time = 0;

    // Normalized pixel coordinates (from 0 to 1)
    vec2 p = vec2(v_LocalPosition) / v_UVCoords.yx.xy.yx;
    
    // Time varying pixel color
    vec3 col = 2.5 + 3.4*cos(u_Time*.05+p.xyx+vec3(3,0,6));
    
    col *= p.x / p.y + sin(u_Time*.05+vec3(0,4,0));

    // Output to screen
    vec4 wavyColor = vec4(col, .5);

    EntityID = v_EntityID;
    FragColor = texture(u_TextureSlots[v_TextureSlot], v_UVCoords) * v_TintColor * wavyColor;       
}