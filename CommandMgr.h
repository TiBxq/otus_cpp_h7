#pragma once

#include "CommandHandler.h"

class CommandMgr
{
public:
    CommandMgr(int n) : N(n)
    {}

    void Add(CommandBase* cmd)
    {
        if (!m_pkg)
        {
            m_pkg = new StaticCommandPackage(this, N);
        }
        m_pkg->Add(cmd);
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
            std::cout << "Completed" << std::endl;

            for (CommandHandler* handler : m_handlers)
            {
                handler->Handle(m_pkg);
            }

            delete m_pkg;
            m_pkg = nullptr;
        }
    }

    void AddHandler(CommandHandler* handler)
    {
        m_handlers.emplace_back(handler);
    }

    //dtor

//private:
    std::vector<CommandBase*> m_cmdList;
    std::vector<CommandHandler*> m_handlers;
    int N;
    int counter = 0;

    CommandPackage* m_pkg = nullptr;
};