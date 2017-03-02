in vec4 vertex;
in vec4 color;
out vec4 v_Color;
uniform mat4 matrix;

void main(void)
{
    v_Color = color;
    gl_Position = matrix * vertex;
}