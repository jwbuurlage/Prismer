#pragma once

#include <memory>
#include <string>

namespace Arya
{
    using std::string;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::make_unique;

    class Material;
    class Geometry;

    // Basicly a texture with all glyphs on it and associated information
    class Font
    {
        public:
            Font();
            ~Font();

            bool loadFromFile(string filename);

            // generates a grayscale material with the text on it
            shared_ptr<Material> renderText(string text);

            // generates a Geometry that has the text on it
            shared_ptr<Geometry> createTextGeometry(string text);

            shared_ptr<Material> getFontMaterial();

        private:
            //pimpl idiom to avoid including stb truetype here
            struct FontInfo;
            unique_ptr<FontInfo> info;

            static const int BitmapWidth = 512;
            static const int BitmapHeight = 512;
            static const int fontHeight = 48; //all 256 glyphs must fit on bitmap
    };

}
