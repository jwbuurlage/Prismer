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
// - Label text is aligned to left side of label and clipped to size of label
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

            //! Check if a point is inside the view. Used for mouse clicks.
            //! pos is normalized [-1,1] screen coordinates
            //! pixelScaling is 1/windowsize
            bool isInside(const vec2& pos, const vec2& pixelScaling);

            // Called by Graphics
            // This should include parent and properly compute absolute/relative stuff
            // screenOffset is offset to MIDDLE of quad
            virtual vec2 getScreenSize(const vec2& pixelScaling);
            virtual vec2 getScreenOffset(const vec2& pixelScaling);

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

            //! Setting font to zero causes no text to be rendered
            //! Initially a default font is chosen
            void setFont(shared_ptr<Font> font);

            void setText(const string& text);
            const string& getText() const { return text; }

            //width of the line in pixels
            //TODO
            float getLineWidth() { return 8.0f * text.size(); }

            // The text geometry is in pixels starting from bottom-left
            // instead of a [-1,1] quad
            // This offset points to the bottom-left point of the label
            vec2 getScreenSize(const vec2& pixelScaling) override;
            vec2 getScreenOffset(const vec2& pixelScaling) override;

            Material* getFontMaterial() { return material.get(); }
            Geometry* getTextGeometry() { return geometry.get(); }
        private:
            string text;

            shared_ptr<Font> font;
            shared_ptr<Material> material;
            shared_ptr<Geometry> geometry;
    };

    class TextBox : public View
    {
        public:
            TextBox(const this_is_private&);
            ~TextBox();

            static shared_ptr<TextBox> create();

            void setFont(shared_ptr<Font> font) { label->setFont(font); }

            void setText(const string& text) { label->setText(text); }
            const string& getText() const { return label->getText(); }

            void setBackground(shared_ptr<Material> mat) { background->setMaterial(mat); }
            shared_ptr<Material> getBackground() const { return background->material; }

            // no cursor by default
            void setCursor(shared_ptr<Material> mat) { cursor->setMaterial(mat); }

            //! Whether it can capture focus (default true)
            void setEnabled(bool enabled = true);

            //! Focus is only possible if also enabled
            //! Default is false, user can click to give focus
            void setFocus(bool focus = true);

            //! Called by Graphics
            Label* getTextLabel() { return label.get(); }
        private:
            bool isEnabled;
            bool hasFocus;

            InputBinding clickBinding;
            InputBinding textBinding;

            shared_ptr<ImageView> background;
            shared_ptr<ImageView> cursor;
            shared_ptr<Label> label; // holds current text

            bool onClick(const MousePos& pos);
            bool onCharacter(char character);

            void updateCursorPos();
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

            //helper function for Views
            const vec2& getPixelScaling() const { return pixelScaling; }
        private:
            shared_ptr<View> root;
            shared_ptr<Font> defaultFont;

            vec2 pixelScaling;
    };
}

