#pragma once
#include "Resources.h"
#include <GL/glew.h>

namespace Arya
{
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

        private:
            shared_ptr<Texture> loadResource( string filename );

            void loadDefaultTexture(); //Generates default texture
            void loadWhiteTexture(); //Generates white texture for overlay
    };
}
