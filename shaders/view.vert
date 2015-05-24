#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 vertexPosition; //[-1,-1]x[-1,1] quad
layout (location = 1) in vec2 texturePosition;

uniform vec2 screenOffset; //offset to MIDDLE of quad
uniform vec2 screenSize;   //size of quad in [0,1] range
uniform int isFont;

out vec2 texCoo;

void main()
{
    texCoo = texturePosition;

    //vertexPosition is one of the vertices of the [-1,1]x[-1,1] quad
    //translate quad from [-1,1] range to a [0,1]x[-1,0] range
    //so quad top-left is at origin
    //vec2 pos = vec2(0.5 * vertexPosition.x + 0.5, 0.5 * vertexPosition.y - 0.5);

    gl_Position = vec4(screenOffset + screenSize * vertexPosition, 0.0, 1.0);
}
