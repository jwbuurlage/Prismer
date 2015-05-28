#pragma once

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "AryaBinding.h"

namespace Arya
{
    using std::deque;
    using std::function;
    using std::map;
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

            void bind(const string& command, function<void()> f);
            void unbind(const string& command);

            // Toggle visiblity of console
            // is also bound to tilde but can be done manually as well
            void toggleConsole();
        private:
            // Console output history
            deque<string> history;
            void addOutputLine(const string& line);

            // Console key binding
            InputBinding bindTilde;
            InputBinding bindShiftTilde;

            // Command bindings
            void handleConsoleInput(string line);
            map<string, function<void()>> bindings;

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
