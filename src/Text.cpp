#include "Text.h"
#include "Locator.h"
#include "Files.h"
#include "Materials.h"
#include "Textures.h"
#include "Geometry.h"
#include "common/Logger.h"
#include <GL/glew.h>
#define STB_RECT_PACK_IMPLEMENTATION
#include "common/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "common/stb_truetype.h"

namespace Arya
{
    struct Font::FontInfo
    {
        bool valid;
        int fontHeight; // in pixels
        shared_ptr<Material> material;
        stbtt_fontinfo fontInfo;
        stbtt_packedchar charInfo[256];
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

    bool Font::loadFromFile(string filename, int fontHeight)
    {
        File* fontfile = Locator::getFileSystem().getFile(string("fonts/") + filename);
        if (!fontfile)
        {
            LogWarning << "Font not found: " << filename << endLog;
            return false;
        }

        if (!stbtt_InitFont(&info->fontInfo, (unsigned char*)fontfile->getData(), 0))
        {
            LogWarning << "Unable to load font: " << filename << endLog;
            return false;
        }

        // Then we can use the following to generate bitmaps of single characters,
        // i.e. the texture data for a single character.
        //      stbtt_GetCodepointBitmap()           -- allocates and returns a bitmap
        //      stbtt_MakeCodepointBitmap()          -- renders into bitmap you provide
        //      stbtt_GetCodepointBitmapBox()        -- how big the bitmap must be
        // Together with some functions that give the proper positioning and metrics
        //
        // Option 2
        // Instead, we use the stbtt packing api to render all glyphs onto a large texture
        // with associated information, and store the texture in info->material.
        // (This will use the option-1 functions internally)
        //
        // Read about oversampling
        // https://github.com/nothings/stb/tree/master/tests/oversample

        info->fontHeight = fontHeight;

        unsigned int first_unicode_char = 0;
        const int characterCount = 256;

        const int width = BitmapWidth;
        const int height = BitmapHeight;
        unsigned char pixeldata[width*height];

        stbtt_pack_context pc;
        if (!stbtt_PackBegin(&pc, pixeldata, width, height, 0, 1, NULL))
        {
            LogWarning << "Unable to load font " << filename << ". Error in PackBegin." << endLog;
            return false;
        }

        unsigned int oversample = 2; // 1 is no oversampling
        stbtt_PackSetOversampling(&pc, oversample, oversample);

        if (!stbtt_PackFontRange(&pc, (unsigned char*)fontfile->getData(), 0,
                    (float)fontHeight,
                    first_unicode_char, characterCount, info->charInfo))
            LogWarning << "Unable to load pack font glyphs on bitmap for font " << filename << endLog;

        stbtt_PackEnd(&pc);

        GLuint handle;
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        //linear sampling is important for oversampling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
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

        // Scale factor, see stbtt file
        float scale = stbtt_ScaleForPixelHeight(&info->fontInfo, info->fontHeight);

        // Vertical alignment
        // ascent - extension above baseline
        // descent - extension below baseline
        // linegap - distance from descent to next rows ascent
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&info->fontInfo, &ascent, &descent, &lineGap);

        float newlineAdvance = scale * (ascent - descent + lineGap);

        // For each character there is a quad, meaning 2 triangles
        // A triangle is 3 vertices, with x,y,s,t each (position,texture)
        GLfloat* vertexData = new GLfloat[text.length() * 2 * 3 * 4];

        int index = 0;
        float xpos = 0.0f, ypos = newlineAdvance;
        float minY =  10000.0f;
        float maxY = -10000.0f;
        stbtt_aligned_quad q;
        //char prevCharacter = 0;
        for (unsigned int i = 0; i < text.size(); i++)
		{
            int codepoint = text[i];

            //UTF-8
            if (text[i] & 0x80) //if highest bit set, its a multi-byte character
            {
                if ((text[i] & 0xE0) == 0xC0) // if ???xxxxx is 110xxxxx ==> 2-byte character
                {
                    if (i+1 == text.size())
                    {
                        LogWarning << "Invalid UTF-8 string '" << text << '\'' << endLog;
                        continue;
                    }
                    codepoint = int(text[i] & 0x1F);
                    i++;
                    codepoint = (codepoint << 6) | int(text[i] & 0x3F);
                }
                else if ((text[i] & 0xF0) == 0xE0) // if ????xxxx is 1110xxxx ==> 3-byte character
                {
                    if (i+2 == text.size())
                    {
                        LogWarning << "Invalid UTF-8 string '" << text << '\'' << endLog;
                        continue;
                    }
                    codepoint = int(text[i] & 0x0F);
                    i++;
                    codepoint = (codepoint << 6) | int(text[i] & 0x3F);
                    i++;
                    codepoint = (codepoint << 6) | int(text[i] & 0x3F);
                }
                else
                    LogWarning << "Rendering font with unkown (more than 3 bytes) UTF-8 character '" << text << '\'' << endLog;
            }


            //if (prevCharacter)
            //{
            //    xpos += scale*stbtt_GetCodepointKernAdvance(&info->fontInfo, prevCharacter, character);
            //    prevCharacter = character;
            //}
            //
            //int advance, lsb;
            //float x_shift = xpos - (float)floor(xpos);
            //stbtt_GetCodepointHMetrics(&info->fontInfo, character, &advance, &lsb);

            if (codepoint == '\n')
            {
                ypos += newlineAdvance;
                xpos = 0.0f;
                continue;
            }

            // This will advance xpos appropriately
            stbtt_GetPackedQuad(info->charInfo, BitmapWidth, BitmapHeight, codepoint, &xpos, &ypos, &q, 0);

            // a---d
            // |   |
            // b---c
            //
            // q.?0 is top-left
            // q.?1 is bottom-right
            // stbtt has y downwards, hence the sign

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

            if (-q.y0 > maxY) maxY = -q.y0;
            if (-q.y1 < minY) minY = -q.y1;
		}

        auto geometry = make_shared<Geometry>();

        geometry->primitiveType = GL_TRIANGLES;
        geometry->vertexCount = index/4;
        geometry->indexCount = 0;
        geometry->frameCount = 1;

        geometry->minX = vertexData[0];
        geometry->minY = minY;
        geometry->minZ = 0.0f;
        geometry->maxX = xpos;
        geometry->maxY = maxY;
        geometry->maxZ = 0.0f;

        geometry->createVertexBuffer();
        geometry->setVertexBufferData(index * sizeof(GLfloat), vertexData);
        delete[] vertexData;

        geometry->createVAOs(1);
        geometry->bindVAO(0);
        geometry->setVAOdata(0, 2, 4 * sizeof(GLfloat), 0);
        geometry->setVAOdata(1, 2, 4 * sizeof(GLfloat), 8);

        return geometry;
    }

    float Font::getLineAdvance()
    {
        float scale = stbtt_ScaleForPixelHeight(&info->fontInfo, info->fontHeight);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&info->fontInfo, &ascent, &descent, &lineGap);
        return scale * (ascent - descent + lineGap);
    }
}
