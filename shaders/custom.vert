#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 vertexPosition;

uniform mat4 mMatrix;
uniform mat4 vpMatrix;

out vec2 texCoo;
out vec4 worldPos;

void main()
{
    texCoo = vertexPosition.xy;
    worldPos = mMatrix * vec4(vertexPosition, 1.0);
    gl_Position = vpMatrix * worldPos;
}
