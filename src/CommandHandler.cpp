#include <algorithm>
#include "CommandHandler.h"
#include "common/Logger.h"

namespace Arya
{
    void CommandHandler::bind(const string& command, function<void(const string&)> f)
    {
        //make lowercase
        string cmd(command);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        auto pos = cmd.find(' ');
        if (pos != string::npos)
        {
            LogWarning << "Trying to bind invalid command '" << command << "' containing spaces." << endLog;
            return;
        }

        auto x = bindings.find(cmd);
        if (x != bindings.end())
            LogWarning << "Rebinding command '" << command << "'" << endLog;
        bindings[cmd] = f;
    }

    void CommandHandler::unbind(const string& command)
    {
        string cmd(command);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        auto x = bindings.find(cmd);
        if (x == bindings.end())
            LogWarning << "Trying to unbind command that does not exist '" << command << "'" << endLog;
        else
            bindings.erase(x);
    }

    int CommandHandler::executeCommand(const string& line)
    {
        string cmd(line);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        // Erase everything after the first space
        auto pos = cmd.find(' ');
        if (pos != string::npos)
            cmd.erase(pos);

        auto x = bindings.find(cmd);
        if (x == bindings.end())
        {
            LogWarning << "Unkown command '" << cmd << "'" << endLog;
            return 0;
        }
        x->second(line);
        return 1;
    }
}
