//type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_RenderMode;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_RenderMode;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_RenderMode = a_RenderMode;
}

//type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_RenderMode;

uniform sampler2D u_Textures[32];

const float glyphCenter = 0.35;

void main()
{
    vec4 sampled = texture2D(u_Textures[int(v_TexIndex)], v_TexCoord);
    float dist = sampled.r;

    if (v_RenderMode == 0)
    {
        color = vec4(v_Color.x, v_Color.y, v_Color.z, dist);
    }
    else 
    {
        float width = fwidth(dist);
        float alpha = smoothstep(glyphCenter - width, glyphCenter + width, dist);

        color = vec4(v_Color.xyz, alpha);
        if (color.a < 0.02)
        {
            discard;
        }
    }
}