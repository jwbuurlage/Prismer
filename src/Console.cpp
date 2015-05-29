#include "Console.h"
#include "Interface.h"
#include "Materials.h"
#include "Locator.h"
#include "Text.h"
#include "InputSystem.h"
#include "CommandHandler.h"
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

        auto font = make_shared<Font>();
        font->loadFromFile("DejaVuSansMono.ttf", 14);

        const float boxHeight = 30.0f;
        float lineHeight = (font ? font->getLineAdvance() : 18.0f);
        float labelHeight = lineHeight * lineCount;
        float backgroundHeight = 10.0f + labelHeight + 10.0f + boxHeight + 10.0f;

        background = ImageView::create();
        background->setMaterial(matGray);
        background->setPosition(vec2(0.0f, 1.0f), vec2(0.0f, -(0.5f*backgroundHeight+10.0f))); //middle-top + (0, -10px)
        background->setSize(vec2(1.0f, 0.0f), vec2(-20.0f, backgroundHeight)); //fullwidth + (-20px, +300px)
        background->addToRootView();

        textBox = TextBox::create();
        if (font) textBox->setFont(font);
        textBox->setBackground(matGrayDark);
        textBox->setCursor(matGrayDark2);
        textBox->setPosition(vec2(0.0f, -1.0f), vec2(0.0f, 0.5f*boxHeight + 10.0f));
        textBox->setSize(vec2(1.0f, 0.0f), vec2(-20.0f , boxHeight));
        textBox->setEnabled(false);
        background->add(textBox);

        consoleOutputLabel = Label::create();
        consoleOutputLabel->setPosition(vec2(0.0f, 1.0f), vec2(0.0f, -10.0f -0.5f*labelHeight));
        consoleOutputLabel->setSize(vec2(1.0f, 0.0f), vec2(-20.0f, labelHeight));
        if (font) consoleOutputLabel->setFont(font);
        background->add(consoleOutputLabel);

        consoleVisible = false;
        background->setVisible(false);
        auto bindFunc = [this](bool down,const MousePos&){ if (down) toggleConsole(); return down; };
        bindTilde = Locator::getInputSystem().bind("tilde", bindFunc, CHAIN_LAST);
        bindShiftTilde = Locator::getInputSystem().bind("shift+tilde", bindFunc, CHAIN_LAST);

        textBox->setCallback([this](bool isEnter) {
                    //either enter or escape
                    //was pressed in console textbox
                    if (isEnter)
                    {
                        if (&Arya::Locator::getCommandHandler())
                            Arya::Locator::getCommandHandler().executeCommand(textBox->getText());
                    }
                    else
                    {
                        toggleConsole(); //on escape
                    }
                    textBox->setText("");
                });

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

        int firstLine = 0;
        if (history.size() > lineCount)
            firstLine = history.size() - lineCount;

        bool flag = false;
        string visibleText;
        for (unsigned int i = firstLine; i < history.size(); i++)
        {
            if (flag) visibleText.append(1, '\n');
            else flag = true;
            //TODO: if line length too long, then
            //split the line
            visibleText.append(history[i]);
        }

        consoleOutputLabel->setText(visibleText);

        graphicsInitialized = true;
    }
}
