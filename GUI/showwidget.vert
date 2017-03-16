#version 430 core
layout (location = 1) in vec4 vertex;
layout (location = 2) in vec4 color;

layout (location = 3) uniform mat4 matrix;

out vec4 v_Color;
mat4 genScaleMatrix(vec4 c);
mat4 genTranslateMatrix(vec4 c);
mat4 genRotateMatrix(float degree, vec4 axis);

void main(void)
{
    mat4 scaleMatrix = genScaleMatrix(vec4(1.0, 1.0, 1.0, 0.0));
    // mat4 translateMatrix = genTranslateMatrix(vec4(0.0, 0.0, 0.5, 0.0));
    mat4 roateMatrix = genRotateMatrix(45.0f, vec4(0.0f, 0.0f, 1.0f, 0.0f));
    gl_Position = matrix  * scaleMatrix  *vertex;
    v_Color = color;
}

/**
* 生成一个缩放矩阵,
* 每个方向的缩放由c的x,y,z分量决定
*/
mat4 genScaleMatrix(vec4 c)
{
    mat4 result = mat4(
        c.x, 0.0, 0.0, 0.0,
        0.0, c.y, 0.0, 0.0,
        0.0, 0.0, c.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    return transpose(result);
}

/**
* 生成一个平移矩阵
* 平移的距离由c的x,y,z分量决定 
*/
mat4 genTranslateMatrix(vec4 c)
{
    mat4 result = mat4(
        1.0, 0.0, 0.0, c.x,
        0.0, 1.0, 0.0, c.y,
        0.0, 0.0, 1.0, c.z,
        0.0, 0.0, 0.0, 1.0
    );
    return transpose(result);
}

/**
* 生成一个围绕向量 axis 旋转 degree 度的旋转矩阵
* axis 的取值有三种
* vec4(1.0f, 0.0f, 0.0f, 0.0f)
* vec4(0.0f, 1.0f, 0.0f, 0.0f)
* vec4(0.0f, 0.0f, 1.0f, 0.0f)
*/
mat4 genRotateMatrix(float degree, vec4 axis)
{
    float angle = radians(degree);
    mat4 result;
    // 由于opengl按列主序,故下面构造出来的矩阵需要转置
    if (axis - vec4(1.0f, 0.0f, 0.0f, 0.0f) == vec4(0.0f))
    {
        result = mat4(
            1.0,        0.0,         0.0, 0.0,
            0.0, cos(angle), -sin(angle), 0.0,
            0.0, sin(angle),  cos(angle), 0.0,
            0.0,        0.0,         0.0, 1.0
        );
    }
    else if (axis - vec4(0.0f, 1.0f, 0.0f, 0.0f) == vec4(0.0f))
    {
        result = mat4(
            cos(angle), 0.0, -sin(angle), 0.0,
            0.0,        1.0,         0.0, 0.0,
            sin(angle), 0.0,  cos(angle), 0.0,
            0.0,        0.0,         0.0, 1.0
        );
    }
    else if (axis - vec4(0.0f, 0.0f, 1.0f, 0.0f) == vec4(0.0f))
    {
        result = mat4(
            cos(angle), -sin(angle), 0.0, 0.0,
            sin(angle),  cos(angle), 0.0, 0.0,
                   0.0,         0.0, 1.0, 0.0,
                   0.0,         0.0, 0.0, 1.0
        );
    }
    return transpose(result);
}
