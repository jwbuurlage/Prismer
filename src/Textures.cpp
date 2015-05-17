#include "Textures.h"
#include "common/Logger.h"
#include "Files.h"
#include "Locator.h"
#include <sstream>
#include "stb_image.c"

namespace Arya
{
    TextureManager::TextureManager(){
    }

    TextureManager::~TextureManager(){
        cleanup();
    }

    bool TextureManager::init(){
        loadDefaultTexture();
        loadWhiteTexture();
        return true;
    }

    void TextureManager::cleanup(){
        unloadAll();
    }

    Texture* TextureManager::loadResource( string filename ){
        filename.insert(0, "textures/");
        File* imagefile = Locator::getFileSystem().getFile(filename);
        if( imagefile == 0 ) return 0;

        Texture* texture = 0;

        int width, height, channels;
        // NOTE: using STBI_default as last arg gives wrong pixel data
        unsigned char* ptr = stbi_load_from_memory((stbi_uc*)imagefile->getData(), imagefile->getSize(), &width, &height, &channels, STBI_rgb_alpha); 
        if(ptr)
        {
            texture = new Texture;
            texture->width = width;
            texture->height = height;

            glGenTextures(1, &texture->handle);
            glBindTexture(GL_TEXTURE_2D, texture->handle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

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

    Texture* TextureManager::createTextureFromHandle(string name, GLuint handle)
    {
        if(resourceLoaded(name)) return getTexture(name);

        Texture* texture = new Texture;
        addResource(name, texture);
        texture->handle = handle;
        //Get width and height
        glBindTexture(GL_TEXTURE_2D, texture->handle);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (GLint*)&texture->width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&texture->width);
        return texture;
    }

    void TextureManager::loadDefaultTexture(){
        if( resourceLoaded("default") ) return;

        Texture* defaultTex = new Texture;
        defaultTex->handle = 0;
        defaultTex->width = 128;
        defaultTex->height = 128;

        unsigned int pixelCount = defaultTex->width * defaultTex->height;
        float* imageData = new float[pixelCount * 4];
        for(unsigned int i = 0; i < pixelCount; ++i){
            bool purple=(((i%defaultTex->width)/16+(i/defaultTex->width/16))%2==0);
            imageData[4*i+0] = purple?1.0f:0.0f; //red
            imageData[4*i+1] = 0.0f; //green
            imageData[4*i+2] = purple?1.0f:0.0f; //blue
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

    void TextureManager::loadWhiteTexture()
    {
        Texture* whiteTex= new Texture;
        whiteTex->handle = 0;
        whiteTex->width = 4;
        whiteTex->height = 4;
        unsigned int pixelCount = whiteTex->width * whiteTex->height;
        float* imageData = new float[pixelCount * 4];
        for(unsigned int i = 0; i < pixelCount; ++i)
        {
            imageData[4*i+0] = 1.0f; //red
            imageData[4*i+1] = 1.0f; //green
            imageData[4*i+2] = 1.0f; //blue
            imageData[4*i+3] = 1.0f; //alpha
        }
        glGenTextures(1, &whiteTex->handle);
        glBindTexture( GL_TEXTURE_2D, whiteTex->handle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, whiteTex->width, whiteTex->height, 0, GL_RGBA, GL_FLOAT, imageData );

        delete[] imageData;

        addResource("white", whiteTex);
        return;

    }
}

