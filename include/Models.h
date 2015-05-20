//
// Entity has one Model shared between entities, one AnimationState not shared
// Model has multiple Meshes, one AnimationData
//      these Meshes and AnimationData are NOT shared with other Models
// Mesh has one Material and one Geometry
//      Material is always shared
//      Geometry can be shared when Mesh.clone() is used
// 
#pragma once
#include "Resources.h"
#include "AnimationBase.h"
#include "GraphicsComponent.h"

#include <vector>
#include <string>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

#include <glm/glm.hpp>
using glm::vec3;

namespace Arya
{
    class Geometry;
    class Material;
    class AnimationState;
    class ShaderProgram;

    class Mesh
    {
        public:
            // Note that geometry is shared and not unique.
            // One could make a copy of a model and its meshes
            // but let it point to the same geometry but
            // with a different material.
            shared_ptr<Geometry> geometry;
            shared_ptr<Material> material;
    };

    enum ModelType //Note: Same constats as in the AryaModel file format
    {
        ModelTypeUnkown = 0,
        ModelTypeStatic = 1,
        VertexAnimated = 2,
        BoneAnimated = 3
    };

    class Model
    {
        public:
            Model(ModelType type);
            virtual ~Model();

            const ModelType modelType;

            const vector<Mesh*>& getMeshes() const { return meshes; }
            ShaderProgram* getShaderProgram() const { return shaderProgram.get(); }

            //! Entity::setModel will call this function
            //! to create the appropriate subclass of AnimationState
            unique_ptr<AnimationState> createAnimationState();

            vec3 getBoundingBoxVertex(int vertexNumber);

            //! Sets the material on all Meshes
            void setMaterial(shared_ptr<Material> mat);

        private:
            friend class ModelManager;
            Mesh* createMesh();

            vector<Mesh*> meshes;
            shared_ptr<ShaderProgram> shaderProgram;
            unique_ptr<AnimationData> animationData;

            float minX; // Values needed to define
            float maxX; // bounding box for model.
            float minY;
            float maxY;
            float minZ;
            float maxZ;
    };

    class ModelManager : public ResourceManager<Model>
    {
        public:
            ModelManager();
            ~ModelManager();

            bool init();
            void cleanup();

            shared_ptr<Model> getModel(string filename){ return getResource(filename); }
        private:
            shared_ptr<Model> loadResource(string filename );

            void loadPrimitives();

            shared_ptr<ShaderProgram> staticShader;
            shared_ptr<ShaderProgram> animatedShader;
            shared_ptr<ShaderProgram> primitiveShader;
    };
}
