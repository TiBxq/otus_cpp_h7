#pragma once

#include <vector>
#include <ctime>

class CommandMgr;
class CommandBase;

class CommandPackage
{
public:
    CommandPackage(CommandMgr* mgr) : m_time(0), m_mgr(mgr)
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