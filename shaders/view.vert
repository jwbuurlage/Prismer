#version 330
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 vertexPosition; //[-1,-1]x[-1,1] quad

uniform vec2 screenOffset; //offset to MIDDLE of quad
uniform vec2 screenSize;   //size of quad in [0,1] range

out vec2 texCoo;

void main()
{
    //vertexPosition is one of the vertices of the [-1,1]x[-1,1] quad
    //the texcoo should be in [0,1]x[0,1] range
    //AND have y reversed because textures work like that

    //translate quad from [-1,1] range to a [0,1]x[-1,0] range
    //so quad top-left is at origin

    vec2 pos = vec2(0.5 * vertexPosition.x + 0.5, 0.5 * vertexPosition.y - 0.5);

    //reverse y for texture
    texCoo.x = pos.x;
    texCoo.y = -pos.y;

    gl_Position = vec4(screenOffset + screenSize * vertexPosition, 0.0, 1.0);
}
