#version 330
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex;
uniform sampler2D shadowMap;
uniform mat4 lightMatrix;
uniform vec4 customUniform;

in vec2 texCoo;
in vec4 worldPos;

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(tex, texCoo);

    float r = dot(texCoo,texCoo);
    if (r > 0.35)
    {
        float a = (r-0.35)/(1.0-0.35);
        // r interval [0.35 , 1] now mapped to [0,1]
        // map [0,1] to [1,customUniform]
        fragColor *= a*customUniform + (1.0-a)*vec4(1.0);
    }

    vec4 posOnShadowTex = lightMatrix * worldPos;

    if(!(posOnShadowTex.x < 0.0 || posOnShadowTex.x > 1.0))
        if(!(posOnShadowTex.y < 0.0 || posOnShadowTex.y > 1.0)) 
            if(!(posOnShadowTex.z < 0.0 || posOnShadowTex.z > 1.0)) 
                if(texture(shadowMap, posOnShadowTex.xy).r + 0.001 < posOnShadowTex.z)
                    fragColor *= 0.5;
}
