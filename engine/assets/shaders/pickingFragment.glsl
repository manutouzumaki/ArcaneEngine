#version 330 core

uniform sampler2D uTextures[8];

in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;
in float fObjID;

out vec3 color;

void main()
{
    vec4 texColor = vec4(1, 1, 1, 1);
    if(fTexID > 0.0f)
    {
        int id = int(fTexID);
        texColor = fColor * texture(uTextures[id], fTexCoords);
    }

    if(texColor.a < 0.5)
    {
        discard;
    }
    
    color = vec3(fObjID, fObjID, fObjID);
}
