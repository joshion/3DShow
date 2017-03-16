#version 430 core

uniform sampler2D tex;
in vec2 vs_tex_coord;
out vec4 color;
// in vec4 v_Color;

void main(void)
{
    color = texture(tex, vs_tex_coord);
    // gl_FragColor = v_Color;
}