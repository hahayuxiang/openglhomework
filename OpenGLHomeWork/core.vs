#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 transform;
void main()
{
gl_Position = transform * vec4(position, 1.0f);//变成齐次坐标系，方便坐标变换
}