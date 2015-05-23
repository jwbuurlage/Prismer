#include "Text.h"
#include "Locator.h"
#include "Files.h"
#include "Materials.h"
#include "Textures.h"
#include "Geometry.h"
#include "common/Logger.h"

#include <GL/glew.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "common/stb_truetype.h"

namespace Arya
{
    struct Font::FontInfo
    {
        bool valid;
        shared_ptr<Material> material;
        stbtt_bakedchar charInfo[256];
    };

    Font::Font() : info(make_unique<FontInfo>())
    {
        info->valid = false;
    }

    Font::~Font()
    {
    }

    shared_ptr<Material> Font::getFontMaterial()
    {
        if (info->valid)
            return info->material;
        return nullptr;
    }

    bool Font::loadFromFile(string filename)
    {
        File* fontfile = Locator::getFileSystem().getFile(string("fonts/") + filename);
        if (!fontfile)
        {
            LogWarning << "Font not found: " << filename << endLog;
            return false;
        }

        const int characterCount = 256;

        const int width = BitmapWidth;
        const int height = BitmapHeight;
        unsigned char pixeldata[width*height];

        int res = stbtt_BakeFontBitmap((unsigned char*)fontfile->getData(), 0, fontHeight,
                pixeldata, width, height, 0, characterCount, info->charInfo);
        if (res <= 0 && res != -characterCount)
        {
            LogWarning << "Unable to bake font bitmap for font " << filename << endLog;
            return false;
        }

        GLuint handle;
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixeldata ); 

        info->material = Material::createFromHandle(handle);
        info->valid = true;

        return true;
    }

    shared_ptr<Material> Font::renderText(string text)
    {
        if (!info->valid)
        {
            LogError << "Font::renderText called on invalid font." << endLog;
            return nullptr;
        }

        //TODO
        (void)text;

        return nullptr;
    }

    shared_ptr<Geometry> Font::createTextGeometry(string text)
    {
        if (!info->valid)
        {
            LogError << "Font::createTextGeometry called on invalid font." << endLog;
            return nullptr;
        }

        // For each character there is a quad, meaning 2 triangles
        // A triangle is 3 vertices, with x,y,s,t each (position,texture)
        GLfloat* vertexData = new GLfloat[text.length() * 2 * 3 * 4];

        int index = 0;
        float xpos = 0.0f, ypos = 0.0f;
        stbtt_aligned_quad q;
        for (auto character : text)
		{
			if (character == ' ') 
			{
				xpos += 8.0f;
				continue;
			}

			stbtt_GetBakedQuad(info->charInfo, BitmapWidth, BitmapHeight, character, &xpos, &ypos, &q, true);

            // a---d
            // |   |
            // b---c
            //
            // q.?0 is top-left
            // q.?1 is bottom-right
            // stbtt has y downwards

            //first triangle: a-b-c
            //a
            vertexData[index++] = q.x0;
            vertexData[index++] = -q.y0;
            vertexData[index++] = q.s0;
            vertexData[index++] = q.t0;
            //b
            vertexData[index++] = q.x0;
            vertexData[index++] = -q.y1;
            vertexData[index++] = q.s0;
            vertexData[index++] = q.t1;
            //c
            vertexData[index++] = q.x1;
            vertexData[index++] = -q.y1;
            vertexData[index++] = q.s1;
            vertexData[index++] = q.t1;

            //second triangle: c-d-a
            //c
            vertexData[index++] = q.x1;
            vertexData[index++] = -q.y1;
            vertexData[index++] = q.s1;
            vertexData[index++] = q.t1;
            //d
            vertexData[index++] = q.x1;
            vertexData[index++] = -q.y0;
            vertexData[index++] = q.s1;
            vertexData[index++] = q.t0;
            //a
            vertexData[index++] = q.x0;
            vertexData[index++] = -q.y0;
            vertexData[index++] = q.s0;
            vertexData[index++] = q.t0;

            xpos = q.x1;
		}

        auto geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = text.length() * 6;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(index * sizeof(GLfloat), vertexData);
        delete[] vertexData;

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 2, 4 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 4 * sizeof(GLfloat), 8);

        return geometry;
    }
}
