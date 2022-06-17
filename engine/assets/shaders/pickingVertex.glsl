#version 330 core

layout (location=0) in vec2 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTex;
layout (location=3) in float aTexID;
layout (location=4) in float aObjID;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexID;
out float fObjID;

void main()
{
    fColor = aColor;
    fTexCoords = aTex;
    fTexID = aTexID;
    fObjID = aObjID;
    gl_Position = uProj * uView * vec4(aPos, 0.0f, 1.0f);
}
