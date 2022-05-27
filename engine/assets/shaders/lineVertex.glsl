#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 fColor;

void main()
{
    fColor = vec4(aColor, 1.0f);
    gl_Position = uProj * uView * vec4(aPos, 1.0f);
}

