#pragma once
#include "Resources.h"
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Arya
{
    using glm::vec4;

    class Texture
    {
        public:
            Texture(){ handle = 0; width = 0; height = 0; }
            ~Texture(){ if( handle ) glDeleteTextures(1, &handle); }

            GLuint handle;
            GLuint width;
            GLuint height;
            //we could add more info about
            //bit depths and mipmap info and so on
    };

    class TextureManager : public ResourceManager<Texture>
    {
        public:
            TextureManager();
            ~TextureManager();

            bool init();
            void cleanup();

            //If no texture found it will return 0
            shared_ptr<Texture> getTexture( string filename ){ return getResource(filename); }

            shared_ptr<Texture> createTextureFromHandle(string name, GLuint handle);

            //This texture will not be stored under a name
            //so make sure to keep the shared_ptr
            shared_ptr<Texture> createTexture(const vec4& color);

        private:
            shared_ptr<Texture> loadResource( string filename );

            void loadDefaultTexture(); //Generates default texture
    };
}
