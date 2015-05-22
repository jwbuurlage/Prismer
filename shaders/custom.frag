#version 330
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex;
uniform vec4 customUniform;

in vec2 texCoo;

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = customUniform*texture(tex, texCoo);
}
