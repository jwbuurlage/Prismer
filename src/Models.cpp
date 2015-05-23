#include "Models.h"
#include "Files.h"
#include "Geometry.h"
#include "Locator.h"
#include "Materials.h"
#include "AnimationVertex.h"
#include "Shaders.h"
#include "common/Logger.h"
#include <map>
#include <algorithm>

using std::map;
using std::make_pair;

//=============================================================================
// Part of the Arya model file format

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

//=============================================================================

namespace Arya
{

//=============================================================================
//Model

    Model::Model(ModelType type) : modelType(type)
    {
        boundingMin = vec3(0.0f);
        boundingMax = vec3(0.0f);
    }

    Model::~Model()
    {
        for(unsigned int i = 0; i < meshes.size(); ++i)
            delete meshes[i];
    }

    shared_ptr<Model> Model::create(string filename)
    {
        return Locator::getModelManager().getModel(filename);
    }

    unique_ptr<AnimationState> Model::createAnimationState()
    {
        if(animationData == 0) return 0;
        return animationData->createAnimationState();
    }

    vec3 Model::getBoundingBoxVertex(int vertexNumber)
    {
        switch(vertexNumber)
        {
        //    case 0: return vec3(minX, minY, minZ);
        //    case 1: return vec3(minX, minY, maxZ);
        //    case 2: return vec3(minX, maxY, minZ);
        //    case 3: return vec3(minX, maxY, maxZ);
        //    case 4: return vec3(maxX, minY, minZ);
        //    case 5: return vec3(maxX, minY, maxZ);
        //    case 6: return vec3(maxX, maxY, minZ);
        //    case 7: return vec3(maxX, maxY, maxZ);
            default: break;
        }
        return vec3(0,0,0);
    }    

    Mesh* Model::createMesh()
    {
        Mesh* mesh = new Mesh;
        meshes.push_back(mesh);
        return mesh;
    }

    void Model::setMaterial(shared_ptr<Material> mat)
    {
        for (auto mesh : meshes)
            mesh->material = mat;
    }

    shared_ptr<Model> Model::clone()
    {
        shared_ptr<Model> copy = make_shared<Model>(modelType);

        for (auto mesh : meshes)
            copy->meshes.push_back(new Mesh(*mesh));
        copy->shaderProgram = shaderProgram;
        copy->animationData = animationData;

        return copy;
    }

//=============================================================================
//ModelManager

    ModelManager::ModelManager()
    {
    }

    ModelManager::~ModelManager()
    {
        cleanup();
    }

    bool ModelManager::init()
    {
        // Load shaders
        staticShader = make_shared<ShaderProgram>(
                "../shaders/basic.vert",
                "../shaders/basic.frag");
        if (!staticShader->isValid()) {
            staticShader = nullptr;
            LogError << "Could not load basic shader." << endLog;
            return false;
        }
        staticShader->enableUniform(UNIFORM_MOVEMATRIX | UNIFORM_VPMATRIX | UNIFORM_TEXTURE);

        animatedShader = make_shared<ShaderProgram>(
                "../shaders/vertexanimatedmodel.vert",
                "../shaders/vertexanimatedmodel.frag");
        if (!animatedShader->isValid()) {
            animatedShader = nullptr;
            LogError << "Could not load vertexanimatedmodel shader." << endLog;
            return false;
        }
        animatedShader->enableUniform(UNIFORM_MOVEMATRIX | UNIFORM_VIEWMATRIX | UNIFORM_VPMATRIX | UNIFORM_TEXTURE | UNIFORM_MATERIALPARAMS | UNIFORM_ANIM_INTERPOL);
        // TODO - Move this out of the engine
        animatedShader->addUniform3fv("tintColor", [](Entity*){ return vec3(0.5, 1.0, 0.5); });

        primitiveShader = make_shared<ShaderProgram>(
                "../shaders/basic.vert",
                "../shaders/basic.frag");
        if (!primitiveShader->isValid()) {
            primitiveShader = nullptr;
            LogError << "Could not load primitive shader." << endLog;
            return false;
        }
        primitiveShader->enableUniform(UNIFORM_MOVEMATRIX | UNIFORM_VPMATRIX | UNIFORM_TEXTURE);

        loadPrimitives();

        return true;
    }

    void ModelManager::cleanup()
    {
        unloadAll();
    }

    shared_ptr<Model> ModelManager::loadResource(string filename)
    {
        File* modelfile = Locator::getFileSystem().getFile(string("models/") + filename);
        if( modelfile == 0 ) return nullptr;

        //Note: except for the first magic int
        //this loader does not check the integrity of the data
        //This means that it could crash on invalid files

        char* pointer = modelfile->getData();

        AryaHeader* header = (AryaHeader*)pointer;

        shared_ptr<Model> model = nullptr;

        do{ //for easy break on errors

            if( (modelfile->getSize() < sizeof(AryaHeader) + sizeof(SubmeshInfo)) || header->magic != ARYAMAGICINT )
            {
                LogError << "Not a valid Arya model file: " << filename << endLog;
                break;
            }

            if( header->modeltype < 1 || header->modeltype > 2 )
            {
                LogError << "Arya model with unkown modeltype: " << header->modeltype << endLog;
                break;
            }

            if( header->frameCount < 1 )
            {
                LogError << "Arya model with invalid number of frames: " << header->frameCount << endLog;
                break;
            }

            model = make_shared<Model>((ModelType)header->modeltype);

            //All materials in the file
            vector< shared_ptr<Material> > materials;

            LogDebug << "Loading model " << filename << " with " << header->submeshCount << " meshes." << endLog;

            //Parse all materials
            pointer += sizeof(AryaHeader);
            pointer += header->submeshCount*sizeof(SubmeshInfo);

            char* nameBuf = new char[512];
            for(int m = 0; m < header->materialCount; ++m)
            {
                //Get string
                int count = 0;
                nameBuf[0] = *pointer++;
                while(nameBuf[count]){ ++count; nameBuf[count] = *pointer++; }
                nameBuf[count++] = '.';
                nameBuf[count++] = 't';
                nameBuf[count++] = 'g';
                nameBuf[count++] = 'a';
                nameBuf[count++] = 0;
                
                shared_ptr<Material> mat = Locator::getMaterialManager().getMaterial(nameBuf);
                materials.push_back(mat);
            }

            //Parse animations

            //slight hack with raw-pointer to leave ownership at Model
            shared_ptr<VertexAnimationData> animData;

            int animationCount = *(int*)pointer; pointer += 4;
            if(!animationCount)
            {
                model->shaderProgram = staticShader;
                model->animationData = nullptr;
                LogDebug << "Model has no animations" << endLog;
            }
            else
            {
                LogDebug << "Model has " << animationCount << " animations in " << header->frameCount << " frames: ";

                model->shaderProgram = animatedShader;

                animData = make_shared<VertexAnimationData>();
                model->animationData = animData;

                VertexAnim newAnim;
                for(int anim = 0; anim < animationCount; ++anim)
                {
                    //Get string
                    int count = 0;
                    nameBuf[0] = *pointer++;
                    while(nameBuf[count]){ ++count; nameBuf[count] = *pointer++; }

                    LogDebug << nameBuf << " ";

                    newAnim.frameTimes.clear();
                    newAnim.startFrame = *(int*)pointer; pointer += 4;
                    newAnim.endFrame = *(int*)pointer; pointer += 4;
                    for(int i = 0; i <= (newAnim.endFrame-newAnim.startFrame); ++i)
                    {
                        newAnim.frameTimes.push_back( *(float*)pointer );
                        pointer += 4;
                    }

                    //Only add the animation if there are actually enough frames
                    if( newAnim.startFrame < header->frameCount && newAnim.endFrame < header->frameCount )
                        animData->animations.insert(make_pair(nameBuf, newAnim));
                    else
                        LogDebug << "(not enough frames) ";
                }

                LogDebug << endLog;

            }

            delete[] nameBuf;

            float* boundingBoxData = (float*)pointer;
            pointer += 6*sizeof(float);
            model->boundingMin.x = boundingBoxData[0];
            model->boundingMax.x = boundingBoxData[1];
            model->boundingMin.y = boundingBoxData[2];
            model->boundingMax.y = boundingBoxData[3];
            model->boundingMin.z = boundingBoxData[4];
            model->boundingMax.z = boundingBoxData[5];

            //Parse all geometries
            for(int s = 0; s < header->submeshCount; ++s)
            {
                Mesh* mesh = model->createMesh();
                mesh->material = materials[header->submesh[s].materialIndex];

                shared_ptr<Geometry> geometry = make_shared<Geometry>();
                mesh->geometry = geometry;

                geometry->primitiveType = header->submesh[s].primitiveType;
                geometry->vertexCount = header->submesh[s].vertexCount;
                geometry->indexCount = header->submesh[s].indexCount;
                geometry->frameCount = header->frameCount;

                int floatCount = header->submesh[s].hasNormals ? 8 : 5;
                int frameBytes = geometry->vertexCount * floatCount * sizeof(GLfloat);

                geometry->createVertexBuffer();
                geometry->setVertexBufferData(geometry->frameCount * frameBytes,
                        modelfile->getData() + header->submesh[s].bufferOffset);

                if( geometry->indexCount > 0 )
                {
                    geometry->createIndexBuffer();
                    geometry->setIndexBufferData(sizeof(GLuint) * geometry->indexCount,
                            modelfile->getData() + header->submesh[s].indexbufferOffset);
                }

                //Create a VAO for every frame
                geometry->createVAOs(geometry->frameCount);
                int stride = floatCount * sizeof(GLfloat);

                if( geometry->frameCount == 1 )
                {
                    //Not animated
                    geometry->bindVAO(0);
                    geometry->setVAOdata(0, 3, stride, 0); //pos
                    geometry->setVAOdata(1, 2, stride, 12);//tex
                    if(header->submesh[s].hasNormals)
                        geometry->setVAOdata(2, 3, stride, 20);
                }
                else
                {
                    //Animated
                    //We actually have to parse the list of animations here
                    //because the endFrame of one animation will have startFrame as 'nextFrame'
                    for(int f = 0; f < geometry->frameCount; ++f)
                    {
                        int nextf = (f+1)%geometry->frameCount;
                        if(animData)
                        {
                            for(auto iter : animData->animations)
                            {
                                if( iter.second.endFrame == f )
                                {
                                    nextf = iter.second.startFrame;
                                    break;
                                }
                            }
                        }
                        
                        geometry->bindVAO(f);
                        geometry->setVAOdata(0, 3, stride, f*frameBytes + 0); //pos
                        geometry->setVAOdata(3, 3, stride, nextf*frameBytes + 0); //next pos
                        geometry->setVAOdata(1, 2, stride, f*frameBytes + 12); //tex
                        if(header->submesh[s].hasNormals)
                        {
                            geometry->setVAOdata(2, 3, stride, f*frameBytes + 20); //norm
                            geometry->setVAOdata(4, 3, stride, nextf*frameBytes + 20); //next norm
                        }
                    }
                }
            }

            addResource(filename, model);
        }while(0);

        Locator::getFileSystem().releaseFile(modelfile);
        return model;
    }
}
