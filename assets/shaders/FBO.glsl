//type vertex
#version 460 core

out vec2 v_TexCoord;

void main()
{
    vec4 positions[] = vec4[](
        vec4(0, 0, 0, 1),
        vec4(0, 1, 0, 1),
        vec4(1, 0, 0, 1),
        vec4(1, 1, 0, 1)
    );

    v_TexCoord = positions[gl_VertexID].xy;
    gl_Position = (positions[gl_VertexID] * 2 - 1);
}

//type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    color = texture2D(u_Texture, v_TexCoord);
}