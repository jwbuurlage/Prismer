#pragma once

#include <functional>
#include <map>
#include <string>

namespace Arya
{
    using std::function;
    using std::map;
    using std::string;

    class ImageView;
    class Label;
    class TextBox;

    class CommandHandler
    {
        public:
            CommandHandler() {};
            ~CommandHandler() {};

            bool init();

            //! Bind a (console) command
            //! Commands can not contain spaces
            //! Commands are compared case-insensitive
            //! Handler receives the full line as entered in the console
            //! with original case as typed by user
            void bind(const string& command, function<void(const string&)> f);
            void unbind(const string& command);

            //! Execute a command by calling the handler if registered
            //! Returns 1 if handled, 0 if there is no handler registered
            //! It is automatically called by Console when the user enters a command
            //! in the console.
            int executeCommand(const string& line);
        private:
            map<string, function<void(const string&)>> bindings;
    };
}
