#pragma once

#include <vector>
#include <ctime>
#include <memory>

class CommandMgr;
class CommandBase;

class CommandPackage
{
public:
    CommandPackage(CommandMgr* mgr) : m_time(0), m_mgr(mgr)
    {}

    virtual ~CommandPackage() = default;

    virtual void Add(std::unique_ptr<CommandBase>&& cmd)
    {
        if (m_commands.empty())
        {
            m_time = std::time(0);
        }
        m_commands.emplace_back(std::move(cmd));
    }

    enum class Type
    {
        Static,
        Dynamic
    };
    virtual Type GetType() = 0;

//protected:
    std::vector<std::unique_ptr<CommandBase>> m_commands;
    std::time_t m_time;
    CommandMgr* m_mgr;
};

class StaticCommandPackage : public CommandPackage
{
public:
    StaticCommandPackage(CommandMgr* mgr, std::size_t size) : CommandPackage(mgr), m_size(size)
    {}

    void Add(std::unique_ptr<CommandBase>&& cmd) override;

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