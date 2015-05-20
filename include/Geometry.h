//See Models.h for explanation of Mesh,Material,Geometry system
//Geometry holds the OpenGL data for a piece of geometry
//meaning the vertex buffer objects and so on
//
//This Geometry class is one specific for OpenGL but it should
//be so general that it can be replaced by a DirectX one
#pragma once

#include <GL/glew.h>

namespace Arya
{
    class Geometry
    {
        public:
            Geometry();
            ~Geometry();

            bool isAnimated() const { return frameCount > 1; }

            void createVertexBuffer();
            void createIndexBuffer();
            void setVertexBufferData(int size, void* data);
            void setIndexBufferData(int size, void* data);

            void createVAOs(int frameCount);

            void bindVAO(int vaoIndex);
            void setVAOdata(int attribArrayIndex, int components,
                    int stride, int offset);

            void draw(int frame = 0);

            int frameCount; //1 for static models
            GLsizei vertexCount; //PER FRAME
            GLsizei indexCount;
            GLenum primitiveType;
        private:
            GLuint* vaoHandles; //a list of framecount handles
            GLuint vertexBuffer;
            GLuint indexBuffer;
    };
}
