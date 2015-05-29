///////////////////////////////////////////////////////////////////////////////////////////////
// Interface, important concepts:
// - coordinates are usual mathematical axes
// - position of a View is its MIDDLE point
//
// - Relative position is a vec2 in [-1,1]x[-1,1] that serves as
//   a resolution-independant anchor point with respect to the middle of the parent window
// - Absolute position is the offset in pixels from the relative position
// - Relative size is a vec2 in [0,1]x[0,1] (not [0,2] !) relative to parent window
// - Absolute size is in pixels, added to relative size
//
// - Relative position is relative to MIDDLE of PARENT window
// - Full size of parent is 1.0f
//
// - Label text is aligned to left side of label and clipped to size of label
//
// Example:
// To create a bar at the right side of the screen of 20 px wide, as high as the screen
// - posRel = (1.0, 0.0)   right-middle
// - posAbs = (-10.0, 0.0) Midpoint of bar is 10px to left
// - sizeRel = (0.0, 1.0)   as high as screen
// - sizeAbs = (20.0, 0.0)  20px wide
///////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "ShaderUniformBase.h"
#include "AryaBinding.h"

namespace Arya
{
    using std::function;
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

            //! Create a new View
            static shared_ptr<View> create();

            //! Add a child View to this View
            //! i.e. a label to a window
            void add(shared_ptr<View> child);
            //! Remove a child View from this View
            void remove(shared_ptr<View> child);

            //! Set visibility of View
            //! Visibility also affects child Views, so if a Window is
            //! invisible, then so are its children (buttons, labels)
            void setVisible(bool v) { visible = v; }

            //! Get visibility of View
            bool isVisible() const { return visible; }

            //! Add this View to the Interface root View node
            //! so it will be rendered from now on
            void addToRootView();

            //! Set the position of the View, see top of file for explanation
            void setPosition(const vec2& rel, const vec2& abs);
            //! Set the size of the View, see top of file for explanation
            void setSize(const vec2& rel, const vec2& abs);

            //! Check if a point is inside the view.
            //! Used for mouse clicks.
            //! pos is normalized [-1,1] screen coordinates
            //! pixelScaling is equal to 1.0/windowsize
            bool isInside(const vec2& pos, const vec2& pixelScaling);

            //! Get the effective size and offset of the View including its parent
            //! and including absolute pixel offsets.
            //! Result is in relative coordinates to real screen instead of to parent View
            //! Called by Graphics on each render pass
            //! 
            //! screenOffset is the offset to the MIDDLE of the View
            //! screenSize is size where 1.0 means full screen
            virtual vec2 getScreenSize(const vec2& pixelScaling);
            virtual vec2 getScreenOffset(const vec2& pixelScaling);

            //! Get a vector of child Views
            const vector<shared_ptr<View>>& getChildren() const { return childViews; }

            //! Update the View, called by Root
            //! Used for things like blinking cursor in TextBox
            virtual void update(float elapsedTime) { for(auto c : childViews) c->update(elapsedTime); };
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

    // TODO: replace geometry by material when Font-to-material is finished
    // TODO: clip text to label size
    // TODO: multiline, auto-multiline
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
            float getLineWidth() const;

            //! Called by Graphics
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

            // Callback when Enter or Escape was pressed
            // bool is true if enter, bool is false if escape
            void setCallback(function<void(bool)> f) { callback = f; }

            void setText(const string& text) { label->setText(text); updateCursorPos(); }
            const string& getText() const { return label->getText(); }

            void setFont(shared_ptr<Font> font) { label->setFont(font); }

            void setBackground(shared_ptr<Material> mat) { background->setMaterial(mat); }
            shared_ptr<Material> getBackground() const { return background->material; }

            // no cursor by default
            void setCursor(shared_ptr<Material> mat) { cursor->setMaterial(mat); }

            //! Set whether it can capture focus with mouse click (default true)
            void setEnabled(bool enabled = true);

            //! Focus is only possible if also enabled
            //! Default is false, user can click to give focus
            void setFocus(bool focus = true);

            void update(float elapsedTime) override;
        private:
            bool isEnabled;
            bool hasFocus;

            InputBinding clickBinding;
            InputBinding textBinding;
            function<void(bool)> callback;

            shared_ptr<ImageView> background;
            shared_ptr<ImageView> cursor;
            shared_ptr<Label> label; // holds current text

            float blinkTime;

            bool onClick(const MousePos& pos);
            bool onText(const char* text);

            void updateCursorPos();
    };
    
    class Interface
    {
        public:
            Interface();
            ~Interface();

            //! Add a window to the root node
            void add(shared_ptr<View> child) { root->add(child); }

            //! Remove a window from the root node
            void remove(shared_ptr<View> child) { root->remove(child); }

            //! Initializes Interface, called by Root
            bool init();

            //! Does required computations on window resize, called by Root
            void resize(int windowWidth, int windowHeight);

            //! Updates interfaces like blinking cursors in textviews, called by Root
            void update(float elapsedTime) { root->update(elapsedTime); }

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

