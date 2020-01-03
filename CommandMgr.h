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
            m_pkg = new StaticCommandPackage(this, m_staticPackageSize);
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
                static_cast<DynamicCommandPackage*>(m_pkg)->IncCounter();
            }
        }

        if(!m_pkg)
        {
            m_pkg = new DynamicCommandPackage(this);
        }
    }

    void CloseBlock()
    {
        if (m_pkg)
        {
            if (m_pkg->GetType() == CommandPackage::Type::Dynamic)
            {
                static_cast<DynamicCommandPackage*>(m_pkg)->DecCounter();
            }
        }
    }

    void OnCompleted()
    { 
        if (m_pkg)
        {
            for (auto& handler : m_handlers)
            {
                handler->Handle(m_pkg);
            }

            delete m_pkg;
            m_pkg = nullptr;
        }
    }

    void AddHandler(std::unique_ptr<CommandHandler>&& handler)
    {
        m_handlers.emplace_back(std::move(handler));
    }

    //dtor

private:
    std::vector<std::unique_ptr<CommandHandler>> m_handlers;
    int m_staticPackageSize;

    CommandPackage* m_pkg;
};