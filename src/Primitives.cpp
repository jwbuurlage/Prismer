#include "common/Logger.h"
#include "Models.h"
#include "Geometry.h"

namespace Arya
{
    void ModelManager::loadPrimitives()
    {
        Model* model = new Model(ModelTypeStatic);
        model->animationData = 0;
        model->shaderProgram = primitiveShader;
        model->minX = 0.0f;
        model->maxX = 0.0f;
        model->minY = 0.0f;
        model->maxY = 0.0f;
        model->minZ = 0.0f;
        model->maxZ = 0.0f;

        Mesh* mesh = model->createMesh();
        mesh->setMaterial(0);

        Geometry* geometry = new Geometry();
        mesh->setGeometry(geometry); //adds refcount

        // Vertices
        GLfloat triangleVertices[] = {
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = 3;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(geometry->vertexCount * 3 * sizeof(GLfloat),
                triangleVertices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);

        addResource("triangle", model);
    }
}

