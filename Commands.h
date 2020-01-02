#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

class CommandMgr;

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

class CommandPackage
{
public:
    CommandPackage(CommandMgr* mgr) : m_mgr(mgr), m_time(0)
    {}

    virtual ~CommandPackage() = default;

    virtual void Add(CommandBase* cmd)
    {
        if (m_commands.empty())
        {
            m_time = std::time(0);
        }
        m_commands.emplace_back(cmd);
    }

    virtual void Execute() {}

    enum class Type
    {
        Static,
        Dynamic
    };
    virtual Type GetType() = 0;

    std::vector<CommandBase*> m_commands;
    std::time_t m_time;
    CommandMgr* m_mgr;
};

class StaticCommandPackage : public CommandPackage
{
public:
    StaticCommandPackage(CommandMgr* mgr, std::size_t size) : CommandPackage(mgr), m_size(size)
    {}

    void Add(CommandBase* cmd) override;

    Type GetType() override { return Type::Static; }

private:
    std::size_t m_size;
};

class DynamicCommandPackage : public CommandPackage
{
public:
    DynamicCommandPackage(CommandMgr* mgr) : CommandPackage(mgr)
    {}

    void IncCounter() { ++m_packCounter; }
    void DecCounter();

    Type GetType() { return Type::Dynamic; }

private:
    std::size_t m_packCounter = 1;
};