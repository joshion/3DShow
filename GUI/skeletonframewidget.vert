#version 430 core

layout (location = 1) in vec4 in_position;
//layout (location = 2) in vec4 color;
//out vec4 v_Color;
layout (location = 2) in vec2 in_tex_coord;
out vec2 vs_tex_coord;
layout (location = 3) uniform mat4 matrix;

void main(void)
{
    gl_Position = matrix * in_position;
    //v_Color = color;
    vs_tex_coord = in_tex_coord;
}