#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>
#include <GL/glew.h>

using namespace std;

typedef struct{
    int materialIndex;
    int primitiveType;
    int vertexCount; //per frame
    int hasNormals;
    int indexCount;
    int bufferOffset;
    int indexbufferOffset;
} SubmeshInfo;

typedef struct{
    int magic;
    int modeltype; //1 static, 2 vertex animated, 3 bone animated
    int submeshCount;
    int materialCount;
    int frameCount; //1 for static meshes
    SubmeshInfo submesh[0];
} AryaHeader;

#define ARYAMAGICINT (('A' << 0) | ('r' << 8) | ('M' << 16) | ('o' << 24))

const float a = 0.5f * sqrt(3.0f);

int main(int argc, char* argv[])
{
    //unused parameters
    (void)argc;
    (void)argv;

    string outputFilename("triangle.aryamodel");

    //
    //thick triangle with normals
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
    GLfloat thickTriangleVertices[] = { //x,y,z,nx,ny,nz
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
    GLuint lineIndices[] = {
        4,5, 5,3, 3,4, // top
        2,1, 1,0, 0,2, // bottom
        1,4, 2,5, 0,3  // sides
    };

    // upper bound for file size
    int sizeBound = sizeof(AryaHeader) + 2 * sizeof(SubmeshInfo) + 40 + 4 + 24 + 4 + sizeof(thickTriangleVertices) + sizeof(thickTriangleIndices) + sizeof(lineIndices);

    char* outBuffer = new char[sizeBound];

    char* pointer = outBuffer;

    //Output data start
    AryaHeader* outHeader = (AryaHeader*)pointer;

    outHeader->magic = ARYAMAGICINT;
    outHeader->modeltype = 1; // static
    outHeader->submeshCount = 2; // triangle itself and line strokes
    outHeader->materialCount = 2;
    outHeader->frameCount = 1;

    // First submesh: the triangle itself
    outHeader->submesh[0].materialIndex = 0;
    outHeader->submesh[0].primitiveType = GL_TRIANGLES;
    outHeader->submesh[0].vertexCount = 6;
    outHeader->submesh[0].hasNormals = 1;
    outHeader->submesh[0].indexCount = 24; // 8 faces, 3 vertices per face
    outHeader->submesh[0].bufferOffset = 0;
    outHeader->submesh[0].indexbufferOffset = 0;

    // Second submesh: the line strokes
    outHeader->submesh[1].materialIndex = 1;
    outHeader->submesh[1].primitiveType = GL_LINES;
    outHeader->submesh[1].vertexCount = 6;
    outHeader->submesh[1].hasNormals = 0;
    outHeader->submesh[1].indexCount = 18; // 9 lines, 2 endpoints per line
    outHeader->submesh[1].bufferOffset = 0;
    outHeader->submesh[1].indexbufferOffset = 0;

    pointer += sizeof(AryaHeader);
    pointer += 2*sizeof(SubmeshInfo);
    //end of submesh info

    //
    // Material list
    //
    string mat1("triangle.mat");
    string mat2("triangleline.mat");
    for(int i = 0; i < (int)mat1.length(); ++i) *pointer++ = mat1[i];
    *pointer++ = 0; //zero terminated
    for(int i = 0; i < (int)mat2.length(); ++i) *pointer++ = mat2[i];
    *pointer++ = 0; //zero terminated

    // Animation info
    *(int*)pointer = 0; pointer += 4; //zero animations

    // Bounding box info
    *(float*)pointer =    -a; pointer += 4; // minX
    *(float*)pointer =     a; pointer += 4; // maxX
    *(float*)pointer = -0.5f; pointer += 4; // minY
    *(float*)pointer =  1.0f; pointer += 4; // maxY
    *(float*)pointer =  0.0f; pointer += 4; // minZ
    *(float*)pointer =  1.0f; pointer += 4; // maxZ


    // Now output the vertex buffer
    
    // 4-byte aligned
    int offset;
    offset = (int)(pointer - outBuffer);
    offset = (offset + 3) & (~3);
    pointer = outBuffer + offset;

    outHeader->submesh[0].bufferOffset = offset;
    outHeader->submesh[1].bufferOffset = offset;

    memcpy(pointer, thickTriangleVertices, sizeof(thickTriangleVertices));
    pointer += sizeof(thickTriangleVertices);

    // Output the index buffer of the triangle
    offset = (int)(pointer - outBuffer); // should still be 4-byte aligned
    outHeader->submesh[0].indexbufferOffset = offset;

    memcpy(pointer, thickTriangleIndices, sizeof(thickTriangleIndices));
    pointer += sizeof(thickTriangleIndices);

    // Output the index buffer of the lines
    offset = (int)(pointer - outBuffer);
    outHeader->submesh[1].indexbufferOffset = offset;
    memcpy(pointer, lineIndices, sizeof(lineIndices));
    pointer += sizeof(lineIndices);
    
    ofstream outputFile;
    outputFile.open(outputFilename.c_str(), ios::binary);
    if (!outputFile.is_open())
    {
        cerr << "Unable to open output file: " << outputFilename << endl;
    }
    else
    {
        int totalFileSize = (int)(pointer - outBuffer);
        outputFile.write(outBuffer, totalFileSize);
        outputFile.close();

        cout << "Output written to " << outputFilename << endl;
    }

    delete[] outBuffer;

    return 0;
}

//void doTriangle()
//{
//    geometry->createVertexBuffer();
//    geometry->setVertexBufferData(sizeof(thickTriangleVertices), thickTriangleVertices);
//    geometry->createIndexBuffer();
//    geometry->setIndexBufferData(sizeof(thickTriangleIndices), thickTriangleIndices);
//
//    geometry->createVAOs(1);
//    geometry->bindVAO(0);
//    geometry->setVAOdata(0, 3, 6 * sizeof(GLfloat), 0);
//    geometry->setVAOdata(1, 2, 6 * sizeof(GLfloat), 0); //xy are also st
//    geometry->setVAOdata(2, 3, 6 * sizeof(GLfloat), 3*sizeof(GLfloat)); //xy are also st
//
//    model = make_shared<Model>(ModelTypeStatic);
//    model->shaderProgram = primitiveShader;
//    mesh = model->createMesh();
//    mesh->geometry = geometry;
//    addResource("thicktriangle", model);
//}
