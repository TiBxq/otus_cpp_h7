#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Command
{
public:
    Command(std::string cmd) : m_cmd(cmd)
    {
    }

//private:
    std::string m_cmd;
};

class CommandMgr
{
public:
    void Add(Command&& cmd)
    {
        m_cmdList.emplace_back(std::move(cmd));
    }

    void Execute()
    {
        std::cout << "bulk ";
        for (auto it = m_cmdList.begin(); it != m_cmdList.end();)
        {
            std::cout << it->m_cmd << std::endl;
            it = m_cmdList.erase(it);
        }
    }

//private:
    std::vector<Command> m_cmdList;
};

int main(int, char const* argv[])
{
    int N = std::stoi(argv[1]);

    std::cout << N << std::endl;

    CommandMgr mgr;

    std::string str;
    while (std::getline(std::cin, str))
    {
        //std::cout << str << std::endl;
        mgr.Add(Command(str));

        if (mgr.m_cmdList.size() == N)
        {
            mgr.Execute();
        }
    }

    return 0;
}
