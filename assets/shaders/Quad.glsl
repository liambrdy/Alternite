//type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_Color = a_Color;
}

//type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}