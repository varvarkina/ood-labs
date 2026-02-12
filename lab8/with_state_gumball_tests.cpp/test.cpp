#include "../gumball_machine/GumBallMachineWithState.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Return;
using namespace with_state;

class MockGumballMachine : public IGumballMachine {
public:
    MOCK_METHOD(void, ReleaseBall, (), (override));
    MOCK_METHOD(unsigned, GetBallCount, (), (const, override));
    MOCK_METHOD(void, SetSoldOutState, (), (override));
    MOCK_METHOD(void, SetNoQuarterState, (), (override));
    MOCK_METHOD(void, SetSoldState, (), (override));
    MOCK_METHOD(void, SetHasQuarterState, (), (override));
};

class GumballMachineTest : public ::testing::Test {
protected:
    with_state::GumballMachine gumballMachine{ 5 };

    std::streambuf* originalCout{};
    std::ostringstream testOutput;

    void SetUp() override {
        originalCout = std::cout.rdbuf(testOutput.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(originalCout);
    }
};

TEST_F(GumballMachineTest, InitialState) {
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 5 gumballs
Machine is waiting for quarter
)");
}

TEST_F(GumballMachineTest, InitialStateSoldOut) {
    with_state::GumballMachine m(0);
    EXPECT_EQ(m.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 0 gumballs
Machine is sold out
)");
}

TEST_F(GumballMachineTest, InitialStateSingleGumball) {
    with_state::GumballMachine m(1);
    EXPECT_EQ(m.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 1 gumball
Machine is waiting for quarter
)");
}

TEST_F(GumballMachineTest, InsertQuarterChangesStateToHasQuarter) {
    gumballMachine.InsertQuarter();
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 5 gumballs
Machine is waiting for turn of crank
)");
}

TEST_F(GumballMachineTest, DoubleInsertQuarterHasNoEffectOnState) {
    gumballMachine.InsertQuarter();
    gumballMachine.InsertQuarter();
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 5 gumballs
Machine is waiting for turn of crank
)");
}

TEST_F(GumballMachineTest, TurnCrankInHasQuarterDispensesAndReducesInventory) {
    gumballMachine.InsertQuarter();
    gumballMachine.TurnCrank();
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 4 gumballs
Machine is waiting for quarter
)");
}

TEST_F(GumballMachineTest, TurnCrankWithoutQuarterDoesNothing) {
    gumballMachine.TurnCrank();
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 5 gumballs
Machine is waiting for quarter
)");
}

TEST_F(GumballMachineTest, EjectQuarterInHasQuarterPrintsAndReturnsQuarter) {
    gumballMachine.InsertQuarter();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nQuarter returned\n");
}

TEST_F(GumballMachineTest, EjectQuarterInNoQuarterPrintsMessage) {
    // testOutput уже захвачен в SetUp; убедимся что он пуст
    testOutput.str(""); testOutput.clear();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You haven't inserted a quarter\n");
}

TEST_F(GumballMachineTest, SoldOutAfterConsumingAllGumballs) {
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 0 gumballs
Machine is sold out
)");
}

TEST_F(GumballMachineTest, InsertQuarterInSoldOutHasNoEffect) {
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }
    testOutput.str(""); testOutput.clear();
    gumballMachine.InsertQuarter(); 
    EXPECT_EQ(gumballMachine.ToString(), R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: 0 gumballs
Machine is sold out
)");
}

TEST_F(GumballMachineTest, EjectQuarterInSoldOutPrintsCorrectMessages) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        expectedString += "You inserted a quarter\n";
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nA gumball comes rolling out the slot...\n";
    }
    gumballMachine.EjectQuarter();
    expectedString += "Oops, out of gumballs\nYou can't eject, you haven't inserted a quarter yet\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

// Тесты классов состояний

// NoQuarterState
TEST_F(GumballMachineTest, NoQuarterState_InsertQuarter_TransitionsToHasQuarter) {
    MockGumballMachine mock;
    NoQuarterState state(mock);

    EXPECT_CALL(mock, SetHasQuarterState()).Times(1);

    testOutput.str(""); testOutput.clear();
    state.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\n");
}

TEST_F(GumballMachineTest, NoQuarterState_EjectQuarter_PrintsMessage) {
    MockGumballMachine mock;
    NoQuarterState state(mock);

    testOutput.str(""); testOutput.clear();
    state.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You haven't inserted a quarter\n");
}

TEST_F(GumballMachineTest, NoQuarterState_TurnCrank_PrintsMessage) {
    MockGumballMachine mock;
    NoQuarterState state(mock);

    testOutput.str(""); testOutput.clear();
    state.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned but there's no quarter\n");
}

TEST_F(GumballMachineTest, NoQuarterState_Dispense_PrintsMessage) {
    MockGumballMachine mock;
    NoQuarterState state(mock);

    testOutput.str(""); testOutput.clear();
    state.Dispense();
    EXPECT_EQ(testOutput.str(), "You need to pay first\n");
}

TEST_F(GumballMachineTest, NoQuarterState_ToString) {
    MockGumballMachine mock;
    NoQuarterState state(mock);
    EXPECT_EQ(state.ToString(), "waiting for quarter");
}

// HasQuarterState
TEST_F(GumballMachineTest, HasQuarterState_InsertQuarter_PrintsMessage) {
    MockGumballMachine mock;
    HasQuarterState state(mock);

    testOutput.str(""); testOutput.clear();
    state.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You can't insert another quarter\n");
}

TEST_F(GumballMachineTest, HasQuarterState_EjectQuarter_TransitionsToNoQuarter) {
    MockGumballMachine mock;
    HasQuarterState state(mock);

    EXPECT_CALL(mock, SetNoQuarterState()).Times(1);
    testOutput.str(""); testOutput.clear();
    state.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "Quarter returned\n");
}

TEST_F(GumballMachineTest, HasQuarterState_TurnCrank_TransitionsToSold) {
    MockGumballMachine mock;
    HasQuarterState state(mock);

    EXPECT_CALL(mock, SetSoldState()).Times(1);
    testOutput.str(""); testOutput.clear();
    state.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned...\n");
}

TEST_F(GumballMachineTest, HasQuarterState_Dispense_PrintsNoGumball) {
    MockGumballMachine mock;
    HasQuarterState state(mock);

    testOutput.str(""); testOutput.clear();
    state.Dispense();
    EXPECT_EQ(testOutput.str(), "No gumball dispensed\n");
}

TEST_F(GumballMachineTest, HasQuarterState_ToString) {
    MockGumballMachine mock;
    HasQuarterState state(mock);
    EXPECT_EQ(state.ToString(), "waiting for turn of crank");
}

// SoldState
TEST_F(GumballMachineTest, SoldState_InsertQuarter_Prints) {
    MockGumballMachine mock;
    SoldState state(mock);

    testOutput.str(""); testOutput.clear();
    state.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "Please wait, we're already giving you a gumball\n");
}

TEST_F(GumballMachineTest, SoldState_EjectQuarter_Prints) {
    MockGumballMachine mock;
    SoldState state(mock);

    testOutput.str(""); testOutput.clear();
    state.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "Sorry you already turned the crank\n");
}

TEST_F(GumballMachineTest, SoldState_TurnCrank_Prints) {
    MockGumballMachine mock;
    SoldState state(mock);

    testOutput.str(""); testOutput.clear();
    state.TurnCrank();
    EXPECT_EQ(testOutput.str(), "Turning twice doesn't get you another gumball\n");
}

TEST_F(GumballMachineTest, SoldState_Dispense_IfNoBallsAfterRelease_SetsSoldOut) {
    MockGumballMachine mock;
    SoldState state(mock);

    EXPECT_CALL(mock, ReleaseBall()).Times(1);
    EXPECT_CALL(mock, GetBallCount()).WillOnce(Return(0));
    EXPECT_CALL(mock, SetSoldOutState()).Times(1);

    testOutput.str(""); testOutput.clear();
    state.Dispense();
    EXPECT_EQ(testOutput.str(), "Oops, out of gumballs\n");
}

TEST_F(GumballMachineTest, SoldState_Dispense_IfBallsRemain_SetsNoQuarter) {
    MockGumballMachine mock;
    SoldState state(mock);

    EXPECT_CALL(mock, ReleaseBall()).Times(1);
    EXPECT_CALL(mock, GetBallCount()).WillOnce(Return(1));
    EXPECT_CALL(mock, SetNoQuarterState()).Times(1);

    testOutput.str(""); testOutput.clear();
    state.Dispense();
    EXPECT_EQ(testOutput.str(), "");
}

TEST_F(GumballMachineTest, SoldState_ToString) {
    MockGumballMachine mock;
    SoldState state(mock);
    EXPECT_EQ(state.ToString(), "delivering a gumball");
}

// SoldOutState
TEST_F(GumballMachineTest, SoldOutState_InsertQuarter_Prints) {
    MockGumballMachine mock;
    SoldOutState state(mock);

    testOutput.str(""); testOutput.clear();
    state.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You can't insert a quarter, the machine is sold out\n");
}

TEST_F(GumballMachineTest, SoldOutState_EjectQuarter_Prints) {
    MockGumballMachine mock;
    SoldOutState state(mock);

    testOutput.str(""); testOutput.clear();
    state.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You can't eject, you haven't inserted a quarter yet\n");
}

TEST_F(GumballMachineTest, SoldOutState_TurnCrank_Prints) {
    MockGumballMachine mock;
    SoldOutState state(mock);

    testOutput.str(""); testOutput.clear();
    state.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned but there's no gumballs\n");
}

TEST_F(GumballMachineTest, SoldOutState_Dispense_Prints) {
    MockGumballMachine mock;
    SoldOutState state(mock);

    testOutput.str(""); testOutput.clear();
    state.Dispense();
    EXPECT_EQ(testOutput.str(), "No gumball dispensed\n");
}

TEST_F(GumballMachineTest, SoldOutState_ToString) {
    MockGumballMachine mock;
    SoldOutState state(mock);
    EXPECT_EQ(state.ToString(), "sold out");
}

GTEST_API_ int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}