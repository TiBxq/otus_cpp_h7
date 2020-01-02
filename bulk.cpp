#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "CommandMgr.h"
#include "CommandHandler.h"

int main(int argc, char const* argv[])
{
    std::size_t N = 1;
    if (argc == 2)
    {
        N = std::stoi(argv[1]);
    }

    CommandMgr mgr(N);
    mgr.AddHandler(new ConsoleCommandHandler());
    mgr.AddHandler(new FileCommandHandler());
    StringCommandHandler* strHandler = new StringCommandHandler();
    mgr.AddHandler(strHandler);

    std::string str;
    while (std::getline(std::cin, str))
    {
        CommandBase* cmd = nullptr;

        if (str[0] == '{')
        {
            mgr.OpenBlock();
        }
        else if (str[0] == '}')
        {
            mgr.CloseBlock();
        }
        else
        {
            cmd = new Command(str);
            mgr.Add(cmd);
        }
    }

    std::cout << "str: " << strHandler->str() << std::endl;

    return 0;
}
