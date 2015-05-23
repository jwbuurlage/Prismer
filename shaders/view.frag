#version 330
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex;
uniform int isFont;

in vec2 texCoo;

layout (location = 0) out vec4 fragColor;

void main()
{
    if (isFont == 1)
        fragColor = vec4(0.0, 0.0, 0.0, texture(tex, texCoo).r);
    else
        fragColor = texture(tex, texCoo);
}
