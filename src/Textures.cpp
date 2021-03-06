#include "Textures.h"
#include "common/Logger.h"
#include "Files.h"
#include "Locator.h"
#include <sstream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

namespace Arya
{

    Texture::~Texture()
    {
        if( handle )
            glDeleteTextures(1, &handle);
    }

    shared_ptr<Texture> Texture::createFromHandle(GLuint handle)
    {
        shared_ptr<Texture> texture = make_shared<Texture>();
        texture->handle = handle;
        //Get width and height
        glBindTexture(GL_TEXTURE_2D, texture->handle);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (GLint*)&texture->width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&texture->width);
        return texture;
    }

    TextureManager::TextureManager(){
    }

    TextureManager::~TextureManager(){
        cleanup();
    }

    bool TextureManager::init(){
        loadDefaultTexture();
        return true;
    }

    void TextureManager::cleanup(){
        unloadAll();
    }

    shared_ptr<Texture> TextureManager::loadResource( string filename ){
        filename.insert(0, "textures/");
        File* imagefile = Locator::getFileSystem().getFile(filename);
        if( imagefile == 0 ) return 0;

        shared_ptr<Texture> texture = nullptr;

        int width, height, channels;
        // NOTE: using STBI_default as last arg gives wrong pixel data
        unsigned char* ptr = stbi_load_from_memory((stbi_uc*)imagefile->getData(), imagefile->getSize(), &width, &height, &channels, STBI_rgb_alpha); 
        if(ptr)
        {
            texture = make_shared<Texture>();
            texture->width = width;
            texture->height = height;

            glGenTextures(1, &texture->handle);
            glBindTexture(GL_TEXTURE_2D, texture->handle);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

            //high quality, low speed
            glGenerateMipmap(GL_TEXTURE_2D);

            //low quality, high speed
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


            addResource(filename, texture);

            stbi_image_free(ptr);
        }
        else
        {
            LogError << "Unable to read image data of " << filename << ". Reason: " << stbi_failure_reason() << endLog;
        }

        //OpenGL has the image data now
        Locator::getFileSystem().releaseFile(imagefile);
        return texture;
    }

    void TextureManager::loadDefaultTexture(){
        if( resourceLoaded("default") ) return;

        shared_ptr<Texture> defaultTex = make_shared<Texture>();
        defaultTex->handle = 0;
        defaultTex->width = 128;
        defaultTex->height = 128;

        unsigned int pixelCount = defaultTex->width * defaultTex->height;
        float* imageData = new float[pixelCount * 4];
        for(unsigned int i = 0; i < pixelCount; ++i){
            bool darker=(((i%defaultTex->width)/16+(i/defaultTex->width/16))%2==0);
            imageData[4*i+0] = darker?0.8f:0.6f; //red
            imageData[4*i+1] = darker?0.8f:0.6f; //green
            imageData[4*i+2] = darker?0.8f:0.6f; //blue
            imageData[4*i+3] = 0.8f; //alpha
        }

        //glGetError();
        glGenTextures(1, &defaultTex->handle);
        glBindTexture( GL_TEXTURE_2D, defaultTex->handle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, defaultTex->width, defaultTex->height, 0, GL_RGBA, GL_FLOAT, imageData );

        delete[] imageData;

        addResource("default", defaultTex);
        defaultResource = defaultTex;

        return;
    }

    shared_ptr<Texture> TextureManager::createTexture(const vec4& color)
    {
        shared_ptr<Texture> tex = make_shared<Texture>();
        tex->handle = 0;
        tex->width = 1;
        tex->height = 1;
        glGenTextures(1, &tex->handle);
        glBindTexture( GL_TEXTURE_2D, tex->handle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_FLOAT, &color[0]);
        return tex;
    }
}

