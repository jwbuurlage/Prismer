///////////////////////////////////////////////////////////////////////////////////////////////
// Interface, important concepts:
// - coordinates are usual mathematical axes
// - position of a View is its MIDDLE point
//
// - Relative position is a vec2 in [-1,1]x[-1,1] that serves as
//   a resolution-independant anchor point with respect to the middle of the parent window
// - Absolute position is the offset in pixels from the relative position
// - Relative size is a vec2 in [0,1]x[0,1] relative to parent window
// - Absolute size is in pixels, added to relative size
//
// - Relative position is relative to MIDDLE of PARENT window
// - Full size means 1.0f
//
// - Labels are an exception:
//   - Label position is bottom-left of label
//   - Label horizontal size is computed automatically based on string
//
// Example:
// To create a bar at the right side of the screen of 20 px wide, as high as the screen
// - posRel = (1.0, 0.0)   right-middle
// - posAbs = (-20.0, 0.0) 20px to left
// - sizeRel = (0.0, 1.0)   as high as screen
// - sizeAbs = (20.0, 0.0)  20px wide
///////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "ShaderUniformBase.h"
#include "AryaBinding.h"

namespace Arya
{
    using std::string;
    using std::vector;
    using std::weak_ptr;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::make_shared;
    using glm::vec2;

    class Font;
    class Renderer;
    class Material;
    struct MousePos;

    //! Base class for all window,button,textbox etc
    class View : public std::enable_shared_from_this<View> , public ShaderUniformBase
    {
        protected:
            struct this_is_private {};
        public:
            View(const this_is_private&);
            virtual ~View();

            static shared_ptr<View> create();

            void add(shared_ptr<View> child);
            void remove(shared_ptr<View> child);

            // Visibility also affects child Views
            void setVisible(bool v) { visible = v; }
            bool isVisible() const { return visible; }

            // Simply uses Locator to add to Interface
            void addToRootView();

            const vector<shared_ptr<View>>& getChildren() const { return childViews; }

            void setPosition(const vec2& rel, const vec2& abs);
            void setSize(const vec2& rel, const vec2& abs);

            // Called by Graphics
            // This should include parent and properly compute absolute/relative stuff
            // screenOffset is offset to MIDDLE of quad
            vec2 getScreenSize(const vec2& pixelScaling);
            vec2 getScreenOffset(const vec2& pixelScaling);

        protected:
            weak_ptr<View> parent;
            vector<shared_ptr<View>> childViews;
            bool visible;

            friend class Graphics;
            vec2 posRel;
            vec2 posAbs;
            vec2 sizeRel;
            vec2 sizeAbs;
    };

    class ImageView : public View
    {
        public:
            ImageView(const this_is_private&);
            ~ImageView();

            static shared_ptr<ImageView> create();
            static shared_ptr<ImageView> create(shared_ptr<Material> mat);

            void setMaterial(shared_ptr<Material> mat) { material = mat; }

            shared_ptr<Material> material;
    };

    // TODO: replace geometry by material when Font - to - material is finished
    class Material;
    class Geometry;
    class Label : public View
    {
        public:
            Label(const this_is_private&);
            ~Label();

            static shared_ptr<Label> create();

            //! If no font is given, a default font will be used
            void setText(const string& text, shared_ptr<Font> f = 0);

            Material* getMaterial() { return material.get(); }
            Geometry* getGeometry() { return geometry.get(); }
        private:
            string text;

            shared_ptr<Material> material;
            shared_ptr<Geometry> geometry;
    };

    class TextBox : public View
    {
        public:
            TextBox(const this_is_private&);
            ~TextBox();

            static shared_ptr<TextBox> create();

            void setFont(shared_ptr<Font> f);

        private:
            string text;

            InputBinding clickBinding;

            shared_ptr<Font> font;
            shared_ptr<Material> material;
            shared_ptr<Geometry> geometry;

            bool onClick(const MousePos& pos);
    };
    
    class Interface
    {
        public:
            Interface();
            ~Interface();

            bool init();

            void add(shared_ptr<View> child) { root->add(child); }
            void remove(shared_ptr<View> child) { root->remove(child); }

            void resize(int windowWidth, int windowHeight);

            shared_ptr<View> getRootView() { return root; }
            shared_ptr<Font> getDefaultFont() { return defaultFont; }
        private:
            shared_ptr<View> root;
            shared_ptr<Font> defaultFont;
    };
}

