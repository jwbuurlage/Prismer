#include "common/Logger.h"
#include "Models.h"
#include "Geometry.h"

namespace Arya
{
    void ModelManager::loadPrimitives()
    {
        const float a = 0.5f * sqrt(3.0f);

        shared_ptr<Geometry> geometry;
        shared_ptr<Model> model;
        Mesh* mesh;

        // Creates:
        //  - triangle
        //  - quad (-1,-1,0) to (1,1,0)
        //  - hexagon
        //  - quad2d (-1,-1) to (1,1)
        // All z-coordinates are 0

        GLfloat triangleVertices[] = {
            a, -0.5f, 0.0f,
            0.0f, 1.0f, 0.0f,
            -a, -0.5f, 0.0f
        };

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = 3;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(triangleVertices), triangleVertices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 3 * sizeof(GLfloat), 0); //xy are also st

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("triangle", model);

        GLfloat quadVertices[] = {
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            -1.0f,-1.0f, 0.0f,
            1.0f,  1.0f, 0.0f
        };

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLE_STRIP;
        geometry->vertexCount = 4;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(quadVertices), quadVertices);
        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 3 * sizeof(GLfloat), 0);

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("quad", model);

        GLfloat hexagonVertices[] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.5f, a,    0.0f,

            0.0f, 0.0f, 0.0f,
            0.5f, a,    0.0f,
            -0.5f,a,    0.0f,

            0.0f, 0.0f, 0.0f,
            -0.5f,a,    0.0f,
            -1.0f,0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,
            -1.0f,0.0f, 0.0f,
            -0.5f,-a,   0.0f,

            0.0f, 0.0f, 0.0f,
            -0.5f,-a,   0.0f,
            0.5f, -a,   0.0f,

            0.0f, 0.0f, 0.0f,
            0.5f, -a,   0.0f,
            1.0f, 0.0f, 0.0f
        };

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = 6*3;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(hexagonVertices), hexagonVertices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 3 * sizeof(GLfloat), 0);

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("hexagon", model);

        //x,y,s,t
        GLfloat quad2dVertices[] = {
            1.0f, -1.0f, 1.0f, 1.0f,
            1.0f,  1.0f, 1.0f, 0.0f,
            -1.0f,-1.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f
        };

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLE_STRIP;
        geometry->vertexCount = 4;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(quad2dVertices), quad2dVertices);
        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 2, 4 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 4 * sizeof(GLfloat), 8);

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = staticShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("quad2d", model);
    }
}

