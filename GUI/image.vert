#version 430 core

layout (location = 1) in vec4 in_position;
layout (location = 2) in vec2 in_tex_coord;
out vec2 vs_tex_coord;
layout (location = 3) uniform mat4 view_matrix;
layout (location = 4) uniform mat4 projection_matrix;

void main(void)
{
    gl_Position = projection_matrix * in_position * view_matrix ;
    vs_tex_coord = in_tex_coord;
}