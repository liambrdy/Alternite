//type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in float a_TexIndex;
layout (location = 3) in float a_TilingFactor;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position.xy, 0.0, 1.0);
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;

    vec2 uvs[] = vec2[](
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(0, 1)
    );
    v_TexCoord = uvs[gl_VertexID % 4];
}

//type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
    color = texture2D(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}