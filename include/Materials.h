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
            string name;
            Texture* texture;
            string type;
            float specAmp;    // The "amount" of highlights
            float specPow;    // The "sharpness" of highlights
            float ambient;  // The "amount" of ambient lighting
            float diffuse;  // The "amount" of diffuse lighting

            vec4 getParameters() {
                return vec4(specAmp, specPow, ambient, diffuse);
            }
        private:
            friend class MaterialManager;
            friend class ResourceManager<Material>;

            Material(string _name, Texture* _texture, string _type,
                    float _specAmp, float _specPow,
                    float _ambient, float _diffuse)
            {
                name=_name;
                texture=_texture;
                type=_type;
                specAmp=_specAmp;
                specPow=_specPow;
                ambient=_ambient;
                diffuse=_diffuse;
            }
            ~Material(){}
    };

    class MaterialManager : public ResourceManager<Material> {
        public:
            MaterialManager(){}
            ~MaterialManager(){cleanup();}

            bool init();
            void cleanup();

            void loadMaterials(const vector<string>& filenames);

            Material* getMaterial( string filename ){return getResource(filename); }

        private:
            Material* loadResource(string filename);
    };
};
