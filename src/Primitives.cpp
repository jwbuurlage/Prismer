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
        //  - circle (n-gon)
        // All z-coordinates are 0
        // The thick shapes have z between 0 and 1
        //  - thicktriangle
        //
        // staticShader is without normals
        // primitiveShader is with normals

        //
        //triangle
        //
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
        model->shaderProgram = staticShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("triangle", model);

        //
        //quad
        //
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
        model->shaderProgram = staticShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("quad", model);

        //
        //hexagon
        //
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
        model->shaderProgram = staticShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("hexagon", model);

        //
        //quad2d
        //
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

        //
        //circle
        //
        const int ngon = 20;
        GLfloat ngonVertices[ngon][3][3];

        float prevAngle = 0.0f;
        for (int i = 0; i < ngon; i++)
        {
            for (int j = 0; j < 3; j++) ngonVertices[i][j][2] = 0.0f;

            float angle = (2.0f * M_PI * (i+1))/float(ngon);
            ngonVertices[i][0][0] = 0.0f;
            ngonVertices[i][0][1] = 0.0f;
            ngonVertices[i][1][0] = cos(prevAngle);
            ngonVertices[i][1][1] = sin(prevAngle);
            ngonVertices[i][2][0] = cos(angle);
            ngonVertices[i][2][1] = sin(angle);

            prevAngle = angle;
        }

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = ngon*3;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(ngonVertices), ngonVertices);
        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 3 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 3 * sizeof(GLfloat), 0);

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = staticShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("circle", model);

        //
        //thicktriangle
        //
        //with normals.
        //Flat shading using glProvokingVertex
        //
        // bottom  top
        //
        //   1      4 
        //  /-\    /+\ .
        // 2---0  5---3
        //
        // normals for provoking vertex
        // should be LAST in triangle
        // 0 -z
        // 1 -x,+y
        // 2 -y
        // 3 +z
        // 4 +x,+y

        //x,y,z,nx,ny,nz
        GLfloat thickTriangleVertices[] = {
            a,   -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.0f, 1.0f, 0.0f,   -a, 0.5f,  0.0f,
            -a,  -0.5f, 0.0f, 0.0f,-1.0f,  0.0f,
            a,   -0.5f, 1.0f, 0.0f, 0.0f,  1.0f,
            0.0f, 1.0f, 1.0f,    a, 0.5f,  0.0f,
            -a,  -0.5f, 1.0f, 0.0f, 0.0f,  1.0f
        };
        GLuint thickTriangleIndices[] = {
            4,5,3, //top
            3,5,2, //-y face
            0,3,2, //-y face
            3,0,4, //+x face
            0,1,4, //+x face
            5,4,1, //-x face
            2,5,1, //-x face
            2,1,0 //bottom
        };

        geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = 6; //sizeof(thickTriangleVertices)/sizeof(GLfloat)
        geometry->indexCount = 24; //sizeof(thickTriangleIndices)/sizeof(GLuint)
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(sizeof(thickTriangleVertices), thickTriangleVertices);
        geometry->createIndexBuffer();
        geometry->setIndexBufferData(sizeof(thickTriangleIndices), thickTriangleIndices);

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 3, 6 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 6 * sizeof(GLfloat), 0); //xy are also st
        geometry->setVAOdata(2, 3, 6 * sizeof(GLfloat), 3*sizeof(GLfloat)); //xy are also st

        model = make_shared<Model>(ModelTypeStatic);
        model->shaderProgram = primitiveShader;
        mesh = model->createMesh();
        mesh->geometry = geometry;
        addResource("thicktriangle", model);

    }
}

