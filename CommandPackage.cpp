#include "CommandPackage.h"
#include "CommandMgr.h"

void StaticCommandPackage::Add(std::unique_ptr<CommandBase>&& cmd)
{
    CommandPackage::Add(std::move(cmd));
    if (m_commands.size() == m_size)
    {
        m_mgr->OnCompleted();
    }
}

void DynamicCommandPackage::DecCounter()
{
    --m_packCounter;
    if (m_packCounter == 0)
    {
        m_mgr->OnCompleted();
    }
}