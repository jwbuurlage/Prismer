#version 330
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex;

in vec2 texCoo;
flat in vec3 normal;

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(tex, texCoo);

    vec3 lightDirection = normalize(vec3(0.7,0.7,0.5)); //TODO
    float lightFraction = max(0.0,dot(normalize(normal), lightDirection));
    //lightFraction is now guaranteed in [0,1] because both vectors are normalized
    fragColor.xyz *= 0.2 + 0.8*lightFraction;
}
