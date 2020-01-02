#pragma once

#include <iostream>
#include <string>

class CommandBase
{
public:
    virtual void Execute(std::ostream& os) = 0;
};

class Command : public CommandBase
{
public:
    Command(std::string cmd) : m_cmd(cmd)
    {}

    virtual void Execute(std::ostream& os) override
    {
        os << m_cmd;
    }

private:
    std::string m_cmd;
};