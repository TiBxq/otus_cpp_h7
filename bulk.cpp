#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class CommandBase
{
public:
    virtual void Execute() = 0;
};

class Command : public CommandBase
{
public:
    Command(std::string cmd) : m_cmd(cmd)
    {
    }

    virtual void Execute() override
    {
        std::cout << m_cmd;
    }

//private:
    std::string m_cmd;
};

class CommandMgr
{
public:
    CommandMgr(int n) : N(n)
    {}

    void Add(CommandBase* cmd)
    {
        m_cmdList.emplace_back(cmd);

        if (counter == N)
        {
            Execute();
        }
    }

    void Inc()
    {
        ++counter;

    }

    void Execute()
    {
        counter = 0;

        std::cout << "bulk: ";
        for (auto it = m_cmdList.begin(); it != m_cmdList.end();)
        {
            (*it)->Execute();

            std::cout << ", ";
            it = m_cmdList.erase(it);
        }
        std::cout << std::endl;
    }

    //dtor

//private:
    std::vector<CommandBase*> m_cmdList;
    int N;
    int counter = 0;
};

int main(int, char const* argv[])
{
    std::size_t N = std::stoi(argv[1]);

    std::cout << N << std::endl;

    CommandMgr mgr(N);

    int packCounter = 0;

    std::string str;
    while (std::getline(std::cin, str))
    {
        CommandBase* cmd = nullptr;

        if (str[0] == '{')
        {
            if (packCounter == 0)
            {
                mgr.Execute();
            }
            ++packCounter;
        }
        else if (str[0] == '}')
        {
            --packCounter;
            if (packCounter == 0)
            {
                mgr.Execute();
            }
        }
        else
        {
            if (packCounter <= 0)
            {
                mgr.Inc();
            }

            cmd = new Command(str);
            mgr.Add(cmd);
        }
    }

    if (!mgr.m_cmdList.empty())
    {
        mgr.Execute();
    }

    return 0;
}
