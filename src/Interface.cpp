#include "Interface.h"
#include "Root.h"
#include "Text.h"
#include "Locator.h"
#include "InputSystem.h"
#include "Geometry.h"
#include "common/Logger.h"

namespace Arya
{
    View::View(const this_is_private&)
    {
        posRel = vec2(0.0f);
        posAbs = vec2(0.0f);
        sizeRel = vec2(1.0f);
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

    bool View::isInside(const vec2& pos, const vec2& pixelScaling)
    {
        auto offset = getScreenOffset(pixelScaling);
        auto size = getScreenSize(pixelScaling);
        if (pos.x < offset.x - size.x) return false;
        if (pos.x > offset.x + size.x) return false;
        if (pos.y < offset.y - size.y) return false;
        if (pos.y > offset.y + size.y) return false;
        return true;
    }

    vec2 View::getScreenSize(const vec2& pixelScaling)
    {
        vec2 rel = sizeRel;
        if (auto p = parent.lock())
            rel *= p->View::getScreenSize(pixelScaling);
        return rel + sizeAbs * pixelScaling;
    }

    vec2 View::getScreenOffset(const vec2& pixelScaling)
    {
        vec2 pos = posRel;
        if (auto p = parent.lock())
            pos = p->View::getScreenOffset(pixelScaling) + pos * p->View::getScreenSize(pixelScaling);
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
        setFont(Locator::getRoot().getInterface()->getDefaultFont());
    }

    Label::~Label()
    {
    }

    shared_ptr<Label> Label::create()
    {
        return make_shared<Label>(this_is_private{});
    }

    void Label::setFont(shared_ptr<Font> f)
    {
        if (font != f)
        {
            font = f;
            setText(text);
        }
    }

    void Label::setText(const string& t)
    {
        text = t;

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
    }

    vec2 Label::getScreenSize(const vec2& pixelScaling)
    {
        return 2.0f * pixelScaling;
    }

    vec2 Label::getScreenOffset(const vec2& pixelScaling)
    {
        vec2 offsetMiddle = View::getScreenOffset(pixelScaling);
        vec2 size = View::getScreenSize(pixelScaling);
        //basline fix to have all text inside label
        //move the text 3 pixels upwards
        offsetMiddle.y += 3.0f*2.0f*pixelScaling.y;
        return offsetMiddle - size;
    }

    float Label::getLineWidth() const
    {
        if (geometry) return geometry->maxX - geometry->minX;
        else return 0.0f;
    }

    TextBox::TextBox(const this_is_private& a) : View(a)
    {
        isEnabled = true;
        hasFocus = false;
        background = ImageView::create();
        cursor = ImageView::create();
        label = Label::create();

        background->setSize(vec2(1.0f), vec2(0.0f));
        cursor->setVisible(false);
        cursor->setSize(vec2(0.0f), vec2(8.0f, 18.0f));
        label->setSize(vec2(1.0f), vec2(-10.0f, -18.0f)); //text is 5 px from the side of the textbox
    }

    TextBox::~TextBox()
    {
        //children will automatically be deleted by smart pointer semantics
    }

    shared_ptr<TextBox> TextBox::create()
    {
        auto t = make_shared<TextBox>(this_is_private{});
        // This can not be done in the constructor because
        // shared_from_this will not work there
        t->add(t->background);
        t->add(t->cursor);
        t->add(t->label);
        return t;
    }

    void TextBox::setEnabled(bool enabled)
    {
        if (enabled && !isEnabled)
            clickBinding = Locator::getInputSystem().bindMouseButton(
                    [this](MOUSEBUTTON btn, bool down, const MousePos& pos)
                    {
                        if (btn == MOUSEBUTTON_LEFT && down)
                            return onClick(pos);
                        return false;
                    });
        else if (!enabled && isEnabled)
        {
            clickBinding = nullptr;
            setFocus(false);
        }

        isEnabled = enabled;
    }

    void TextBox::setFocus(bool focus)
    {
        if (!isEnabled) return;
        if (focus && !hasFocus)
        {
            hasFocus = true;
            cursor->setVisible(true);
            blinkTime = 0.0f;
            updateCursorPos();
            textBinding = Locator::getInputSystem().bindTextInput([this](const char* text) {
                    return onText(text);
                    }, CHAIN_FIRST);
        }
        else if (!focus && hasFocus)
        {
            hasFocus = false;
            cursor->setVisible(false);
            textBinding = nullptr;
        }
    }

    bool TextBox::onClick(const MousePos& pos)
    {
        if (isInside(vec2(pos.nX, pos.nY),
                    Locator::getRoot().getInterface()->getPixelScaling()))
        {
            setFocus(true);
            LogDebug << "clicked inside" << endLog;
            //clicked inside, so handled
            return true;
        }
        else
        {
            LogDebug << "clicked outside" << endLog;
            bool handled = hasFocus;
            setFocus(false);
            // we only handled it if text WAS focussed before user clicked outside
            return handled;
        }
    }

    bool TextBox::onText(const char* text)
    {
        if (text[0] == '\x08') //backspace
        {
            string cur = getText();
            if (!cur.empty())
            {
                cur.pop_back();
                setText(cur);
            }
        }
        else if (text[0] == '\r') //enter
        {
            if (callback) callback(true);
        }
        else if (text[0] == '\x1B') //escape
        {
            if (callback) callback(false);
        }
        else
        {
            string cur = getText();
            cur.append(text);
            setText(cur);
        }
        return true;
    }

    void TextBox::updateCursorPos()
    {
        cursor->setPosition(vec2(-1.0f, 0.0f),
                vec2(5.0f + 0.5f*8.0f + label->getLineWidth(), 0.0f));
    }

    void TextBox::update(float elapsedTime)
    {
        if (!hasFocus) return;
        blinkTime += elapsedTime;
        while (blinkTime >= 0.5f)
        {
            cursor->setVisible(!cursor->isVisible());
            blinkTime -= 0.5f;
        }

        View::update(elapsedTime);
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
        defaultFont->loadFromFile("courier.ttf", 14);
        // do not exit if font not found
        return true;
    }

    void Interface::resize(int windowWidth, int windowHeight)
    {
        pixelScaling.x = 1.0f/(float)windowWidth;
        pixelScaling.y = 1.0f/(float)windowHeight;
    }
}

