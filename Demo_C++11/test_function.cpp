#include <iostream>
#include <utility>
#include <thread>
#include <functional>
#include <typeinfo>

#include "gtest/gtest.h"

struct Function_Test : testing::Test{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(Function_Test, ref1)
{
    std::function<void (const int &i, int)> f = [](const int &i, int j){
        EXPECT_EQ(i, j);
    };

    int i = 1;
    f(i, 1);

    f(std::ref(i), 1);
}

TEST_F(Function_Test, ref2)
{
    std::function<void (const int &i, int)> equal = [](const int &i, int j){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        EXPECT_EQ(i, j);
    };

    int i = 1;
    std::thread thr1(equal, i, 1);
    i = 2;
    thr1.join();

    i = 3;
    std::thread thr2(equal, std::ref(i), 2);
    i = 2;
    thr2.join();
}

TEST_F(Function_Test, bind)
{
    using namespace std::placeholders;
    auto sub = [](int i, int j) ->int{
        return i - j;
    };

    auto b1 = std::bind(sub, 10, 1);
    EXPECT_EQ(9, b1());

    auto b2 = std::bind(sub, _1, 1);
    EXPECT_EQ(9, b2(10));

    auto b3 = std::bind(sub, _2, _1);
    EXPECT_EQ(9, b3(1, 10));
}

TEST_F(Function_Test, mem_fn)
{
    struct Double{
        int operator()(int i)
        {
            return i * 2;
        }
    };

    Double d;
    auto f = std::mem_fn(&Double::operator());
    std::cout << typeid(f).name() << std::endl;
    EXPECT_EQ(4, f(d, 2));
}

TEST_F(Function_Test, function)
{
    using namespace std::placeholders;
    struct Multiple{
        int value;
        int operator()(int i)
        {
            return i * value;
        }

        int compute(int i, int j)
        {
            return i * j;
        }
    };

    Multiple m = {0};
    std::function<int& (Multiple&)> value = &Multiple::value;
    std::function<int(int)> multiple1     = std::bind(m, _1);
    std::function<int(int)> multiple2     = std::bind(std::ref(m), _1);
    std::function<int(int, int)> compute1 = std::bind(&Multiple::compute, std::ref(m), _1, _2);
    std::function<int(Multiple&, int, int)> compute2 = Multiple::compute;

    value(m) = 10;
    EXPECT_EQ(10,   value(m));
    EXPECT_EQ(0,    multiple1(10));
    EXPECT_EQ(100,  multiple2(10));
    EXPECT_EQ(50,   compute1(5, 10));
    EXPECT_EQ(50,   compute2(m, 5, 10));
}
