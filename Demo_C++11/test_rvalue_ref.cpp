#include <iostream>
#include <string>
#include <utility>
#include <functional>

#include "gtest/gtest.h"

/**右值引用*/
class RValue_Ref_Test : public testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(RValue_Ref_Test, Basic) {
    struct is_rvalue_reference
    {
        bool operator()(std::string &&a) const{
            return true;
        }
        bool operator()(const std::string &a) const{
            return false;
        }
    };

    is_rvalue_reference rvalue;
    std::string a("123");
    EXPECT_EQ(rvalue(a), false);
    EXPECT_EQ(rvalue(std::string("abc")), true);
}

TEST_F(RValue_Ref_Test, DefaultMove) {
    struct A {
        std::string a = "abc";
        int         *b= nullptr;
    };

    A a1;
    a1.a = "123";
    a1.b = new int(100);
    a1 = A();

    EXPECT_EQ(a1.a, "abc");
    EXPECT_EQ(a1.b, nullptr);

    A a2;
    a2.a = "abc123";
    a2.b = new int(100);

    a1 = std::move(a2);
    EXPECT_EQ(a1.a, "abc123");
    EXPECT_EQ(a2.a, "abc");
    EXPECT_EQ(a1.b, a2.b);
}

TEST_F(RValue_Ref_Test, DefineMove) {
    struct A {
        A()
        {
        }

        A(A &&a)
        {
            std::swap(this->a, a.a);
            std::swap(this->b, a.b);
        }

        A& operator=(A &&a)
        {
            std::swap(this->a, a.a);
            std::swap(this->b, a.b);
        }

        std::string a = "abc";
        int         *b= nullptr;
    };

    A a1;
    a1.a = "123";
    a1.b = new int(100);
    a1 = A();
    EXPECT_EQ(a1.a, "abc");
    EXPECT_EQ(a1.b, nullptr);

    A a2;
    a2.a = "abc123";
    a2.b = new int(100);

    a1 = std::move(a2); //调用赋值函数，出现成员变量p指针泄露
    EXPECT_EQ(a1.a, "abc123");
    EXPECT_NE(a1.b, nullptr);
    EXPECT_EQ(a2.a, "abc");
    EXPECT_EQ(a2.b, nullptr);
}

std::string overloaded(const int& x)
{
    return "lvalue";
}
std::string overloaded(int&& x)
{
    return "rvalue";
}

template <typename T>
std::string fn(T &&x)
{
    if (std::is_reference<T>::value)
        return overloaded(x);
    else
        return overloaded(std::forward<T>(x));
}

TEST_F(RValue_Ref_Test, Other) {
    int a;
    EXPECT_EQ(fn(a), "lvalue");
    EXPECT_EQ(fn(std::move(a)), "rvalue");
    EXPECT_EQ(fn(0), "rvalue");

    int &&b = std::move(a);
    EXPECT_EQ(fn(b), "lvalue");
}
