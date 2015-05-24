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

            //! Load a font from a ttf file
            //! @param fontHeight The height of the glyphs in pixels
            //! fontHeigt can not be larger than 50 because the glyphs
            //! will not fit on the bitmap
            bool loadFromFile(string filename, int fontHeight);

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
    };

}
