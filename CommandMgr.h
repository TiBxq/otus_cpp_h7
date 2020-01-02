#pragma once

class CommandBase;

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
                m_pkg = new DynamicCommandPackage(this);
            }
            else if (m_pkg->GetType() == CommandPackage::Type::Dynamic)
            {
                static_cast<DynamicCommandPackage*>(m_pkg)->IncCounter();
            }
        }
        else
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
        std::cout << "Completed" << std::endl;
        delete m_pkg;
        m_pkg = nullptr;
    }

    //dtor

//private:
    std::vector<CommandBase*> m_cmdList;
    int N;
    int counter = 0;

    CommandPackage* m_pkg = nullptr;
};