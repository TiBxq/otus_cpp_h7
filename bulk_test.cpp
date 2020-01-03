#include <gtest/gtest.h>

#include "CommandMgr.h"
#include "CommandPackage.h"
#include "Command.h"

TEST(bulk, bulk_test1)
{
    CommandMgr mgr(3);
    StringCommandHandler* strHandler = new StringCommandHandler();
    mgr.AddHandler(std::unique_ptr<StringCommandHandler>(strHandler));

    mgr.Add(std::make_unique<Command>("cmd1"));
    mgr.Add(std::make_unique<Command>("cmd2"));
    mgr.Add(std::make_unique<Command>("cmd3"));

    ASSERT_EQ(strHandler->str(), "bulk: cmd1, cmd2, cmd3\n");
}

TEST(bulk, bulk_test2)
{
    CommandMgr mgr(3);
    StringCommandHandler* strHandler = new StringCommandHandler();
    mgr.AddHandler(std::unique_ptr<StringCommandHandler>(strHandler));

    mgr.OpenBlock();
    mgr.Add(std::make_unique<Command>("cmd1"));
    mgr.Add(std::make_unique<Command>("cmd2"));
    mgr.Add(std::make_unique<Command>("cmd3"));
    mgr.Add(std::make_unique<Command>("cmd4"));
    mgr.Add(std::make_unique<Command>("cmd5"));
    mgr.CloseBlock();

    ASSERT_EQ(strHandler->str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n");
}

TEST(bulk, bulk_test3)
{
    CommandMgr mgr(3);
    StringCommandHandler* strHandler = new StringCommandHandler();
    mgr.AddHandler(std::unique_ptr<StringCommandHandler>(strHandler));

    mgr.OpenBlock();
    mgr.Add(std::make_unique<Command>("cmd1"));
    mgr.OpenBlock();
    mgr.Add(std::make_unique<Command>("cmd2"));
    mgr.OpenBlock();
    mgr.Add(std::make_unique<Command>("cmd3"));
    mgr.Add(std::make_unique<Command>("cmd4"));
    mgr.CloseBlock();
    mgr.CloseBlock();
    mgr.Add(std::make_unique<Command>("cmd5"));
    mgr.CloseBlock();

    ASSERT_EQ(strHandler->str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n");
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}