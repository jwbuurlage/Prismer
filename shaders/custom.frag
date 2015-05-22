#version 330
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex;
uniform vec4 customUniform;

in vec2 texCoo;

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(tex, texCoo);

    float r = dot(texCoo,texCoo);
    if (r < 0.35)
        fragColor *= customUniform;
    else
    {
        float a = (r-0.35)/(1.0-0.35);
        // r interval [0.35 , 1] now mapped to [0,1]
        // map [0,1] to [customUniform,1]
        fragColor *= (1.0-a)*customUniform + a*vec4(1.0);
    }
}
