#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 mMatrix;
uniform mat4 vpMatrix;

out vec2 texCoo;
flat out vec3 normal;

void main()
{
    texCoo = texturePosition;

    //Do not apply translation to the normal, by setting the last component to 0
    normal = normalize((mMatrix * vec4(vertexNormal, 0.0)).xyz);

    gl_Position = vpMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
