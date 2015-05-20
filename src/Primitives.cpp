#include "common/Logger.h"
#include "Models.h"
#include "Geometry.h"

namespace Arya
{
    void ModelManager::loadPrimitives()
    {
        GLfloat triangleVertices[] = {
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };

        shared_ptr<Geometry> geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = 3;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(triangleVertices), triangleVertices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);

        shared_ptr<Model> model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        Mesh* mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("triangle", model);

        const float a = 0.5f * sqrt(3.0f);
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
        geometry->vertexCount = 3*6;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(hexagonVertices), hexagonVertices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("hexagon", model);
    }
}

