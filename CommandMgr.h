#pragma once

#include <memory>

#include "CommandHandler.h"

class CommandMgr
{
public:
    CommandMgr(int n) : m_staticPackageSize(n), m_pkg(nullptr)
    {}

    void Add(std::unique_ptr<CommandBase>&& cmd)
    {
        if (!m_pkg)
        {
            m_pkg.reset(new StaticCommandPackage(this, m_staticPackageSize));
        }
        m_pkg->Add(std::move(cmd));
    }

    void OpenBlock()
    {
        if(m_pkg)
        {
            if (m_pkg->GetType() == CommandPackage::Type::Static)
            {
                OnCompleted();
            }
            else if (m_pkg->GetType() == CommandPackage::Type::Dynamic)
            {
                static_cast<DynamicCommandPackage*>(m_pkg.get())->IncCounter();
            }
        }

        if(!m_pkg)
        {
            m_pkg.reset(new DynamicCommandPackage(this));
        }
    }

    void CloseBlock()
    {
        if (m_pkg)
        {
            if (m_pkg->GetType() == CommandPackage::Type::Dynamic)
            {
                static_cast<DynamicCommandPackage*>(m_pkg.get())->DecCounter();
            }
        }
    }

    void OnEOF()
    {
        if (m_pkg)
        {
            if (m_pkg->GetType() == CommandPackage::Type::Static)
            {
                OnCompleted();
            }
        }
        m_pkg = nullptr;
    }

    void OnCompleted()
    { 
        if (m_pkg)
        {
            for (auto& handler : m_handlers)
            {
                handler->Handle(m_pkg.get());
            }
            m_pkg = nullptr;
        }
    }

    void AddHandler(std::unique_ptr<CommandHandler>&& handler)
    {
        m_handlers.emplace_back(std::move(handler));
    }

private:
    std::vector<std::unique_ptr<CommandHandler>> m_handlers;
    int m_staticPackageSize;

    std::unique_ptr<CommandPackage> m_pkg;
};