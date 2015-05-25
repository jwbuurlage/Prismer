#include "Console.h"
#include "Interface.h"
#include "Materials.h"
#include "Locator.h"
#include "InputSystem.h"
#include "common/Logger.h"

namespace Arya
{
    Console::Console()
    {
        graphicsInitialized = false;
        logger.setLoggerCallback([this](const string& line){ addOutputLine(line); });
    }

    Console::~Console()
    {
        logger.setLoggerCallback(nullptr);
    }

    bool Console::init()
    {
        if (graphicsInitialized) return true;

        auto matGray = Material::create(vec4(0.8f, 0.8f, 0.8f, 0.8f));
        auto matGrayDark = Material::create(vec4(0.5f, 0.5f, 0.5f, 0.8f));
        auto matGrayDark2 = Material::create(vec4(0.3f, 0.3f, 0.3f, 0.8f));

        const float lineHeight = 16.0f; //pixels
        const int lineCount = 20;
        const float boxHeight = 30.0f;
        const float backgroundHeight = 10.0f + lineCount * lineHeight + 10.0f + boxHeight + 10.0f;

        background = ImageView::create();
        background->setMaterial(matGray);
        background->setPosition(vec2(0.0f, 1.0f), vec2(0.0f, -(0.5f*backgroundHeight+10.0f))); //middle-top + (0, -10px)
        background->setSize(vec2(1.0f, 0.0f), vec2(-20.0f, backgroundHeight)); //fullwidth + (-20px, +300px)
        background->addToRootView();

        textBox = TextBox::create();
        textBox->setBackground(matGrayDark);
        textBox->setCursor(matGrayDark2);
        textBox->setPosition(vec2(0.0f, -1.0f), vec2(0.0f, 0.5f*boxHeight + 10.0f));
        textBox->setSize(vec2(1.0f, 0.0f), vec2(-20.0f , boxHeight));
        textBox->setEnabled(false);
        background->add(textBox);

        for (int i = 0; i < lineCount; i++)
        {
            //auto img = ImageView::create();
            //img->setMaterial(matGrayDark);
            //img->setPosition(vec2(0.0f, 1.0f), vec2(0.0f, -10.0f - lineHeight*i - 1.0f - 0.5f*(lineHeight-1.0f) ));
            //img->setSize(vec2(1.0f, 0.0f), vec2(-20.0f, lineHeight-1.0f));
            //background->add(img);

            auto lbl = Label::create();
            lbl->setPosition(vec2(0.0f, 1.0f), vec2(0.0f, -10.0f - lineHeight*(1 + i)));
            lbl->setSize(vec2(1.0f, 0.0f), vec2(-20.0f, lineHeight)); //10 px from left and right side
            background->add(lbl);
            lines.push_back(lbl);
        }

        consoleVisible = false;
        background->setVisible(false);
        auto bindFunc = [this](bool down,const MousePos&){ if (down) toggleConsole(); return down; };
        bindTilde = Locator::getInputSystem().bind("tilde", bindFunc, CHAIN_LAST);
        bindShiftTilde = Locator::getInputSystem().bind("shift+tilde", bindFunc, CHAIN_LAST);

        graphicsInitialized = true;
        return true;
    }

    void Console::toggleConsole()
    {
        consoleVisible = !consoleVisible;
        if (consoleVisible) updateLabels();
        background->setVisible(consoleVisible);
        textBox->setEnabled(consoleVisible);
        textBox->setFocus(consoleVisible);
    }

    void Console::addOutputLine(const string& line)
    {
        history.push_back(line);
        if (history.size() > 2000) history.pop_front();
        if (graphicsInitialized && consoleVisible) updateLabels();
    }

    void Console::updateLabels()
    {
        // If anything logs while in this function it will crash
        // therefore, disable the graphicsflag during this function.
        // It will still keep the log messages
        graphicsInitialized = false;
        for (unsigned int i = 0; i < lines.size(); i++)
        {
            int j = history.size() - (lines.size()-i);
            if (j >= 0)
            {
                lines[i]->setText(history[j]);
                lines[i]->setVisible(true);
            }
            else
            {
                lines[i]->setVisible(false);
            }
        }
        graphicsInitialized = true;
    }
}
