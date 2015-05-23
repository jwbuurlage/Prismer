#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;

uniform mat4 mMatrix;
uniform mat4 vpMatrix;

out vec2 texCoo;

void main()
{
    texCoo = texturePosition;
    gl_Position = vpMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
