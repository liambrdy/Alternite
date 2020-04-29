//type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;

out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
}

//type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

const float glyphCenter = 0.25;

void main()
{
    vec4 sampled = texture2D(u_Texture, v_TexCoord);
    float dist = sampled.r;
    float width = fwidth(dist);
    float alpha = smoothstep(glyphCenter - width, glyphCenter + width, dist);

    color = vec4(v_Color.xyz, alpha);
    if (color.a < 0.02)
    {
        discard;
    }
}