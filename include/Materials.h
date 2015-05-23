#pragma once
#include "Resources.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

#include <glm/glm.hpp>
using glm::vec4;

namespace Arya
{
    class Texture;

    class Material
    {
        public:
            Material(shared_ptr<Texture> _texture) :
                texture(_texture),
                type("default"),
                specAmp(1.0f),
                specPow(1.0f),
                ambient(0.3f),
                diffuse(0.7f) {};

            ~Material(){}

            static shared_ptr<Material> create(string filename);
            static shared_ptr<Material> create(const vec4& color);

            shared_ptr<Texture> texture;
            string type;
            float specAmp;    // The "amount" of highlights
            float specPow;    // The "sharpness" of highlights
            float ambient;  // The "amount" of ambient lighting
            float diffuse;  // The "amount" of diffuse lighting

            vec4 getParameters() {
                return vec4(specAmp, specPow, ambient, diffuse);
            }
    };

    class MaterialManager : public ResourceManager<Material> {
        public:
            MaterialManager(){}
            ~MaterialManager(){cleanup();}

            bool init();
            void cleanup();

            void loadMaterials(const vector<string>& filenames);

            shared_ptr<Material> getMaterial( string filename ) { return getResource(filename); }

            shared_ptr<Material> createMaterial(const vec4& color);

        private:
            shared_ptr<Material> loadResource(string filename);
    };
};
