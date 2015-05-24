#include "Interface.h"
#include "Root.h"
#include "Text.h"
#include "Locator.h"
#include "InputSystem.h"
#include "common/Logger.h"

namespace Arya
{
    View::View(const this_is_private&)
    {
        posRel = vec2(0.0f);
        posAbs = vec2(0.0f);
        sizeRel = vec2(0.0f);
        sizeAbs = vec2(0.0f);
        visible = true;
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
        // if it already had a parent
        if (auto p = child->parent.lock())
            p->remove(child);

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

    ImageView::ImageView(const this_is_private& a) : View(a)
    {
    }

    ImageView::~ImageView()
    {
    }

    shared_ptr<ImageView> ImageView::create()
    {
        return make_shared<ImageView>(this_is_private{});
    }

    shared_ptr<ImageView> ImageView::create(shared_ptr<Material> mat)
    {
        auto a = ImageView::create();
        a->material = mat;
        return a;
    }

    Label::Label(const this_is_private& a) : View(a)
    {
    }

    Label::~Label()
    {
    }

    shared_ptr<Label> Label::create()
    {
        return make_shared<Label>(this_is_private{});
    }

    void Label::setText(const string& t, shared_ptr<Font> f)
    {
        text = t;
        auto font = f ? f : Locator::getRoot().getInterface()->getDefaultFont();

        if (!text.empty() && font)
        {
            geometry = font->createTextGeometry(text);
            material = font->getFontMaterial();
        }
        else
        {
            geometry = nullptr;
            material = nullptr;
        }

        setSize(vec2(0.0f), vec2(1.0f));
    }

    TextBox::TextBox(const this_is_private& a) : View(a)
    {
        clickBinding = Locator::getInputSystem().bindMouseButton([this](MOUSEBUTTON btn, bool down, const MousePos& pos)
                {
                    if (btn == MOUSEBUTTON_LEFT && down)
                        return onClick(pos);
                    return false;
                });
    }

    TextBox::~TextBox()
    {
    }

    shared_ptr<TextBox> TextBox::create()
    {
        return make_shared<TextBox>(this_is_private{});
    }

    void TextBox::setFont(shared_ptr<Font> f)
    {
        font = f ? f : Locator::getRoot().getInterface()->getDefaultFont();
    }

    bool TextBox::onClick(const MousePos& pos)
    {
        (void)pos;
        return false;
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

    bool Interface::init()
    {
        defaultFont = make_shared<Font>();
        //defaultFont->loadFromFile("DejaVuSans.ttf");
        defaultFont->loadFromFile("courier.ttf", 28);
        // do not exit if font not found
        return true;
    }

    void Interface::resize(int windowWidth, int windowHeight)
    {
        (void)windowWidth;
        (void)windowHeight;
    }
}

