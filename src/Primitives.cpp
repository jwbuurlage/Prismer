#include "common/Logger.h"
#include "Models.h"
#include "Geometry.h"

namespace Arya
{
    void ModelManager::loadPrimitives()
    {
        shared_ptr<Model> model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;

        shared_ptr<Geometry> geometry = make_shared<Geometry>();
        Mesh* mesh = model->createMesh();
        mesh->geometry = geometry;

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

