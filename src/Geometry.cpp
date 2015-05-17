#include "Geometry.h"

namespace Arya
{
    Geometry::Geometry()
    {
        frameCount = 0;
        vaoHandles = 0;
        vertexBuffer = 0;
        vertexCount = 0;
        indexBuffer = 0;
        indexCount = 0;
        primitiveType = 0;
        refCount = 0;
    }

    Geometry::~Geometry()
    {
        if(vaoHandles)
        {
            glDeleteVertexArrays(frameCount, vaoHandles);
            delete[] vaoHandles;
        }
        if(indexBuffer)
            glDeleteBuffers(1, &indexBuffer);
        if(vertexBuffer)
            glDeleteBuffers(1, &vertexBuffer);
    }

    void Geometry::createVertexBuffer()
    {
        if(!vertexBuffer)
            glGenBuffers(1, &vertexBuffer);
    }

    void Geometry::createIndexBuffer()
    {
        if(!indexBuffer)
            glGenBuffers(1, &indexBuffer);
    }

    void Geometry::setVertexBufferData(int size, void* data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void Geometry::setIndexBufferData(int size, void* data)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void Geometry::createVAOs(int count)
    {
        //Delete old handles if they existed
        if(vaoHandles)
        {
            glDeleteVertexArrays(frameCount, vaoHandles);
            delete[] vaoHandles;
        }
        frameCount = count;
        vaoHandles = new GLuint[frameCount];

        glGenVertexArrays(frameCount, vaoHandles);
    }

    void Geometry::bindVAO(int index)
    {
        glBindVertexArray(vaoHandles[index]);
        if(vertexBuffer)
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        if(indexBuffer)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    }

    void Geometry::setVAOdata(int attribArrayIndex, int components,
            int stride, int offset)
    {
        glEnableVertexAttribArray(attribArrayIndex);
        glVertexAttribPointer(attribArrayIndex, components, GL_FLOAT, GL_FALSE,
                stride, reinterpret_cast<GLubyte*>(offset));
    }

    void Geometry::draw(int frame)
    {
        glBindVertexArray(vaoHandles[frame]);
        glDrawArrays(primitiveType, 0, vertexCount);
    }

}
