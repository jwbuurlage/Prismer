#include "Interface.h"
#include "Root.h"
#include "Locator.h"

namespace Arya
{
    View::View(const this_is_private&)
    {
        posRel = vec2(0.0f);
        posAbs = vec2(0.0f);
        sizeRel = vec2(0.0f);
        sizeAbs = vec2(0.0f);
    }

    View::~View()
    {
    }

    shared_ptr<View> View::create()
    {
        return make_shared<View>(this_is_private{});
    }

    void View::add(shared_ptr<View> child)
    {
        childViews.push_back(child);
        child->parent = shared_from_this();
    }

    void View::remove(shared_ptr<View> child)
    {
        for (auto c = childViews.begin(); c != childViews.end(); c++)
        {
            if (*c == child)
            {
                childViews.erase(c);
                break;
            }
        }
    }

    void View::addToRootView()
    {
        Locator::getRoot().getInterface()->add(shared_from_this());
    }

    void View::setPosition(const vec2& rel, const vec2& abs)
    {
        posRel = rel;
        posAbs = abs;
    }

    void View::setSize(const vec2& rel, const vec2& abs)
    {
        sizeRel = rel;
        sizeAbs = abs;
    }

    vec2 View::getScreenSize(const vec2& pixelScaling)
    {
        vec2 rel = sizeRel;
        if (auto p = parent.lock())
            rel *= p->getScreenSize(pixelScaling);
        return rel + sizeAbs * pixelScaling;
    }

    vec2 View::getScreenOffset(const vec2& pixelScaling)
    {
        vec2 pos = posRel;
        if (auto p = parent.lock())
            pos = p->getScreenOffset(pixelScaling) + pos * p->getScreenSize(pixelScaling);
        return pos + 2.0f * posAbs * pixelScaling;
    }

    Image::Image(const this_is_private& a) : View(a)
    {
    }

    Image::~Image()
    {
    }

    shared_ptr<Image> Image::create()
    {
        return make_shared<Image>(this_is_private{});
    }

    shared_ptr<Image> Image::create(shared_ptr<Material> mat)
    {
        auto a = Image::create();
        a->material = mat;
        return a;
    }

    Interface::Interface()
    {
        root = View::create();
        //top-left and full size
        root->setPosition(vec2(0.0f), vec2(0.0f));
        root->setSize(vec2(1.0f), vec2(0.0f));
    }

    Interface::~Interface()
    {
    }

    void Interface::resize(int windowWidth, int windowHeight)
    {
        (void)windowWidth;
        (void)windowHeight;
    }
}

