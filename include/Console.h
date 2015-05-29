#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <vector>
#include "AryaBinding.h"

namespace Arya
{
    using std::deque;
    using std::function;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::string;
    using std::vector;

    class ImageView;
    class Label;
    class TextBox;

    class Console
    {
        public:
            Console();
            ~Console();

            bool init();

            // Toggle visiblity of console
            // Tilde is automatically bound to this,
            // but this function allows manual opening/closing of the console
            void toggleConsole();
        private:
            // Console output history
            deque<string> history;
            void addOutputLine(const string& line);

            // Console key binding
            InputBinding bindTilde;
            InputBinding bindShiftTilde;

            // Graphics
            bool graphicsInitialized;
            bool consoleVisible;
            shared_ptr<ImageView> background;
            shared_ptr<TextBox> textBox;
            shared_ptr<Label> consoleOutputLabel;

            void updateLabels();

            static const int lineCount = 20;
    };
}
