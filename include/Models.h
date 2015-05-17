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

#include <vector>
#include <string>
using std::string;
using std::vector;

#include <glm/glm.hpp>
using glm::vec3;

namespace Arya
{
    class Geometry;
    class Material;

    class Mesh
    {
        public:
            Mesh();
            ~Mesh();

            Geometry* getGeometry() const { return geometry; }
            Material* getMaterial() const { return material; }

            void setGeometry(Geometry* geom);
            void setMaterial(Material* mat);
        private:
            Geometry* geometry;
            Material* material;
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
            const ModelType modelType;

            const vector<Mesh*>& getMeshes() const { return meshes; }
            const AnimationData* getAnimationData() const { return animationData; }

            //! Entity::setModel will call this function
            //! to create the appropriate subclass of AnimationState
            AnimationState* createAnimationState();

            void addRef(){ refCount++; }
            void release(){ refCount--; }

            vec3 getBoundingBoxVertex(int vertexNumber);

        private:
            //Private constructor because only
            //ModelManager is allowed to create Models
            friend class ModelManager;
            friend class ResourceManager<Model>;
            Model(ModelType type);
            virtual ~Model();

            int refCount;

            Mesh* createMesh();

            vector<Mesh*> meshes;
            AnimationData* animationData;

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

            bool initialize();
            void cleanup();

            Model* getModel(string filename){ return getResource(filename); }
        private:
            Model* loadResource(string filename );
    };
}
