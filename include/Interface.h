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

namespace Arya
{
    using std::string;
    using std::vector;
    using std::weak_ptr;
    using std::shared_ptr;
    using std::make_shared;
    using glm::vec2;

    class Renderer;
    class Material;

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

        private:
            weak_ptr<View> parent;
            vector<shared_ptr<View>> childViews;

            friend class Graphics;
            vec2 posRel;
            vec2 posAbs;
            vec2 sizeRel;
            vec2 sizeAbs;
    };

    class Image : public View
    {
        public:
            Image(const this_is_private&);
            ~Image();

            static shared_ptr<Image> create();
            static shared_ptr<Image> create(shared_ptr<Material> mat);

            void setMaterial(shared_ptr<Material> mat) { material = mat; }

            shared_ptr<Material> material;
    };

    class Label : public View
    {
        public:
            Label(const this_is_private&);
            ~Label();

            static shared_ptr<Label> create();

            string text;
    };

    class Interface
    {
        public:
            Interface();
            ~Interface();

            void add(shared_ptr<View> child) { root->add(child); }
            void remove(shared_ptr<View> child) { root->remove(child); }

            void resize(int windowWidth, int windowHeight);

            shared_ptr<View> getRootView() { return root; }
        private:
            shared_ptr<View> root;
    };
}

