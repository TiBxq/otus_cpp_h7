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
    mgr.AddHandler(std::make_unique<ConsoleCommandHandler>());
    mgr.AddHandler(std::make_unique<FileCommandHandler>());

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "{")
        {
            mgr.OpenBlock();
        }
        else if (line == "}")
        {
            mgr.CloseBlock();
        }
        else
        {
            mgr.Add(std::make_unique<Command>(std::move(line)));
        }
    }

    return 0;
}
