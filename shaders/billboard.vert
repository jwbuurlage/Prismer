#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 vertexPosition;

uniform mat4 mMatrix;
uniform mat4 vpMatrix;
uniform vec2 screenOffset;
uniform vec2 screenSize;

out vec2 texCoo;

void main()
{
    texCoo = 0.5 * (vertexPosition + vec2(1.0));
    texCoo.y = 1.0 - texCoo.y;
    gl_Position = vpMatrix * mMatrix * vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position /= gl_Position.w;
    gl_Position += vec4(screenOffset + screenSize * vertexPosition, 0.0, 0.0);
}
