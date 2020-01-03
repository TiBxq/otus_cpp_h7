#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "CommandPackage.h"
#include "Command.h"

class CommandHandler
{
public:
    virtual ~CommandHandler() = default;
    virtual void Handle(CommandPackage* pkg) = 0;
};

class ConsoleCommandHandler : public CommandHandler
{
public:
    void Handle(CommandPackage* pkg) override
    {
        std::cout << "bulk: ";
        for (auto it = pkg->m_commands.begin(); it != pkg->m_commands.end(); ++it)
        {
            (*it)->Execute(std::cout);
            if (std::distance(it, pkg->m_commands.end()) != 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
};

class FileCommandHandler : public CommandHandler
{
public:
    void Handle(CommandPackage* pkg) override
    {
        std::ofstream of("bulk" + std::to_string(pkg->m_time) + ".log");
        of << "bulk: ";
        for (auto it = pkg->m_commands.begin(); it != pkg->m_commands.end(); ++it)
        {
            (*it)->Execute(of);
            if (std::distance(it, pkg->m_commands.end()) != 1)
            {
                of << ", ";
            }
        }
        of << std::endl;
        of.close();
    }
};

class StringCommandHandler : public CommandHandler
{
public:
    void Handle(CommandPackage* pkg) override
    {
        m_stringstream.clear();

        m_stringstream << "bulk: ";
        for (auto it = pkg->m_commands.begin(); it != pkg->m_commands.end(); ++it)
        {
            (*it)->Execute(m_stringstream);
            if (std::distance(it, pkg->m_commands.end()) != 1)
            {
                m_stringstream << ", ";
            }
        }
        m_stringstream << std::endl;
    }

    std::string str() { return m_stringstream.str(); }

private:
    std::ostringstream m_stringstream;
};